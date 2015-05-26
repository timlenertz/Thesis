#include <cstdlib>
#include <iostream>
#include <random>
#include <cmath>
#include <vector>
#include <opencv2/opencv.hpp>

//#include <pcf/core.h>
#include "../pcf/pcf_core/pcf_core.h"

using namespace pcf;
using namespace Eigen;
using namespace cv;

#include "lib/histogram.h"

int seed = 1;


color_image make_view(point_cloud_full& P, point_cloud_full& Q) {
	projection_image_camera cam(
		pose::from_string("4.67801,0.923923,0.532183,-0.0419589,0.664094,0.0471909,0.744978"),
		projection_frustum::symmetric_perspective_fov_x(angle::degrees(60), 1), 600, 600
	);
	camera_range_point_cloud_full<projection_image_camera> rpc(cam);
	rpc.project(P, rgb_color::red);
	rpc.project(Q);
	color_image im = rpc.to_color_image();
	im.flip(true, false);
	return im;
}

auto hi_point_cloud() {
	auto gpc = import_point_cloud("data/hi.ply");
	set_unique_color(gpc.begin(), gpc.end(), 0xffffff);
	//compute_normals(gpc);
	return gpc;

	unorganized_point_cloud_full pc = make_relief_point_cloud(5.0, 1000, seed);
	projection_image_camera cam(
		pose::from_string("0.220013,-4.92678,3.35604,0.912652,0.408613,0.00823912,0.00583399"),
		projection_frustum::symmetric_perspective_fov_x(angle::degrees(60.0), 1.0),
		400, 400
	);
	pc.downsample_projection(cam);
	//return pc;
}

auto lo_point_cloud() {
	auto gpc = import_point_cloud("data/lo.ply");
	set_unique_color(gpc.begin(), gpc.end(), 0xffffff);
	//compute_normals(gpc);
	return gpc;

	unorganized_point_cloud_full pc = make_relief_point_cloud(5.0, 1000, seed);
	projection_image_camera cam(
		pose::from_string("-0.0192693,-5.58523,3.69624,0.899365,0.437096,-0.00847658,0.00411958"),
		projection_frustum::symmetric_perspective_fov_x(angle::degrees(60.0), 1.0),
		200, 200
	);
	pc.downsample_projection(cam);
	pc.randomly_displace_points(std::uniform_real_distribution<float>(-0.01, 0.01));
	pc.rotate_x_axis(angle::degrees(20));
	pc.rotate_y_axis(angle::degrees(10));
	pc.rotate_z_axis(angle::degrees(5));
	pc.move_x(0.2);
	pc.apply_pose();
	//return pc;
}


histogram signed_distance_histogram(const projected_range_image& ri, float mn, float mx, float sz) {
	histogram H(mn, mx, sz);
	const Mat& mat = ri;
	for(cv::MatConstIterator_<float> it = mat.begin<float>(); it != mat.end<float>(); ++it) {
		float d = *it;
		if(d != projected_range_image::invalid_value) H.add(d);
	}
	H.normalize();
	return H;
}

float gaussian(float x, float psi) {
	float g = 1.0f / std::sqrt(two_pi * psi);
	return g * std::exp(-x*x / (2.0f * psi * psi));
}

float gaussian(float x, float y, float psi) {
	float g = 1.0f / (two_pi * psi);
	return g * std::exp(-(x*x + y*y) / (2.0f * psi * psi));
}

float image_value(const intensity_image& P, const Eigen::Vector2i& i, const Eigen::Affine2f& T = Eigen::Affine2f::Identity()) {
	Eigen::Vector2f ti(T * Eigen::Vector2f(i[0], i[1]));
	return P.at_nothrow(ti[0], ti[1]);
}

struct sample_value {
	float p, q;
	Eigen::Vector2i i;
};

using sample = std::vector<sample_value>;

sample make_sample(const intensity_image& P, const intensity_image& Q, const Eigen::Affine2f& T, std::size_t expected = 100) {
	const std::size_t w = P.width(), h = P.height();
	
	sample S;
	std::size_t left = w * h, needed = expected;
	sample_value v;
	for(v.i[1] = 0; v.i[1] < h; ++v.i[1]) for(v.i[0] = 0; v.i[0] < w; ++v.i[0]) {
		v.p = image_value(P, v.i);
		v.q = image_value(Q, v.i, T);
		
		if(v.p != 0 && v.q != 0 && random_integer(left) < needed) {
			S.push_back(v);
			--needed;			
		}
		--left;
	}

	return S;
}


float probability_density_p(float p, const sample& A, float psi) {
	float res = 0;
	for(const auto& a : A) res += gaussian(a.p - p, psi);
	return res / A.size();
}
float probability_density_q(float q, const sample& A, float psi) {
	float res = 0;
	for(const auto& a : A) res += gaussian(a.q - q, psi);
	return res / A.size();
}
float probability_density_pq(float p, float q, const sample& A, float psi) {
	float res = 0;
	for(const auto& a : A) res += gaussian(a.p - p, a.q - q, psi);
	return res / A.size();
}


float log2_zero(float z) {
	if(z == 0) return 0;
	else return std::log2(z);
}




float mutual_information(const intensity_image& P, const intensity_image& Q, const Eigen::Affine2f& T, float psi = 0.1) {
	sample A = make_sample(P, Q, T), B = make_sample(P, Q, T);
	float H_p = 0, H_q = 0, H_pq = 0;

	for(const auto& a : A) {
		H_p -= log2_zero(probability_density_p(a.p, B, psi));
		H_q -= log2_zero(probability_density_q(a.q, B, psi));
		H_pq -= log2_zero(probability_density_pq(a.p, a.q, B, psi));
	}

	return (H_p + H_q - H_pq) / A.size();
}


Eigen::Matrix3f mutual_information_derivative(const intensity_image& P, const intensity_image& Q, const Eigen::Affine2f& T, float scale, float psi = 0.1) {
	sample A = make_sample(P, Q, T), B = make_sample(P, Q, T);

	auto W_q = [&](const auto& b, const auto& a) {
		float det = 0;
		for(const auto& a2 : A) det += gaussian(b.q - a2.q, psi);
		return gaussian(b.q - a.q, psi) / det;
	};
	
	auto W_pq = [&](const auto& b, const auto& a) {
		float det = 0;
		for(const auto& a2 : A) det += gaussian(b.p - a2.p, b.q - a2.q, psi);
		return gaussian(b.p - a.p, b.q - a.q, psi) / det;
	};

	const std::ptrdiff_t components[6][2] = {
		{0, 0}, {0, 1}, {1, 0}, {1, 1}, // linear
		{0, 2}, {1, 2} // translation
	};
	const float component_scales[6] = { 0.1, 0.1, 0.1, 0.1, 1.0, 1.0 };
	
	Eigen::Matrix3f dT = Eigen::Matrix3f::Zero();
	dT(2, 2) = 1;
	for(std::ptrdiff_t i = 0; i < 6; ++i) {
		Eigen::Affine2f T2 = T;
		T2.matrix()(components[i][0], components[i][1]) += scale * component_scales[i];
		//std::cout << "from\n" << T.matrix() << "\nto\n" << T2.matrix() << "\n\n\n\n";
		
		float sum = 0;
		for(const auto& a : A) for(const auto& b : B) {
			auto im = [&](const sample_value& v, const Eigen::Affine2f& T) { return image_value(Q, v.i, T); };
			float term = b.q - a.q;
			term *= W_q(b, a)/psi - W_pq(b, a)/psi;
			term *= (im(b, T2) - im(a, T2)) - (im(b, T) - im(a, T));
			sum += term;
		}
		sum /= B.size();
		
		dT(components[i][0], components[i][1]) = sum;
	}
	
	return dT;
}




Eigen::Affine2f register_images(const intensity_image& P, intensity_image& Q) {
	Eigen::Affine2f T = Eigen::Affine2f::Identity();
	float scale = 10.0;
	for(int i = 0; i < 10; ++i) {
		Q.export_visualization_to_image_file("output/Q"+std::to_string(i)+".png");
		std::cout << "MI = " << mutual_information(P, Q, T) << std::endl;
		Eigen::Matrix3f dT = mutual_information_derivative(P, Q, T, scale);
		{
			Eigen::Affine2f T2 = T;
			T2.matrix() += dT;
			Q.apply_transformation(T2);
		}
		
		scale *= 0.7;
	}
	return T;
}


int main(int argc, const char* argv[]) {
	/*
	if(argc >= 2) seed = std::atoi(argv[1]);


	std::cout << "Loading point clouds" << std::endl;
	auto P = hi_point_cloud();
	auto Q = lo_point_cloud();


	std::cout << "Initial A_P and A_Q..." << std::endl;
	auto A_P = plane::from_string("-0.988378,-0.118857,-0.0947734,0.460469");
	auto A_Q = plane::from_string("-0.972168,-0.0736805,-0.222396,0.363365");	
	
	make_view(P, Q).export_to_image_file("output/initial.png");

	auto P_cam = estimate_optimal_camera(projection_image_camera(A_P, 6.0, 6.0, 100), P);
	auto Q_cam = projection_image_camera(A_Q, 6.0, 6.0, P_cam.image_width());


	std::cout << "Projecting to plane..." << std::endl;
	auto P_rpc = project(P, P_cam);
	auto Q_rpc = project(Q, Q_cam);
	P_rpc.to_projected_range_image().export_visualization_to_image_file("output/P.png");
	Q_rpc.to_projected_range_image().export_visualization_to_image_file("output/Q.png");
	
	
	std::cout << "Filling holes..." << std::endl;
	auto P_ri = P_rpc.fill_holes();
	auto Q_ri = Q_rpc.fill_holes();
	P_ri.export_visualization_to_image_file("output/P_ri.png");
	Q_ri.export_visualization_to_image_file("output/Q_ri.png");
	
	const float D = 0.0025;
	
	{
		auto mm = P_ri.minimum_and_maximum();
		histogram H_P = signed_distance_histogram(P_ri, mm.first, mm.second, (mm.second-mm.first)/1000);
		float P_z = H_P.value_for_bin(H_P.maximal_entropy_threshold()); 
		P_ri.translate_z(-P_z);
	
		P_ri.normalize(-D, +D);
		//Laplacian(P_ri.opencv_matrix(), P_ri.opencv_matrix(), -1, 1);
		P_ri.export_visualization_to_image_file("output/P_ri2.png");
	}
	{
		auto mm = Q_ri.minimum_and_maximum();
		histogram H_Q = signed_distance_histogram(Q_ri, mm.first, mm.second, (mm.second-mm.first)/1000);
		float Q_z = H_Q.value_for_bin(H_Q.maximal_entropy_threshold()); 
		Q_ri.translate_z(-Q_z);
	
		Q_ri.normalize(-D, +D);
		//Laplacian(Q_ri.opencv_matrix(), Q_ri.opencv_matrix(), -1, 1);
		Q_ri.export_visualization_to_image_file("output/Q_ri2.png");
	}
	
	register_images(P_ri, Q_ri);
	*/
	


	intensity_image P(imread("output/cameramanORIG.png"));
	intensity_image Q(imread("output/cameramanROT.png"));

	P.export_visualization_to_image_file("output/P.png");
	register_images(P, Q);
}
