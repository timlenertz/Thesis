#include <cstdlib>
#include <iostream>
#include <random>
#include <cmath>

//#include <pcf/core.h>
#include "../pcf/pcf_core/pcf_core.h"

using namespace pcf;
using namespace Eigen;

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

projection_image_camera orthogonal_projection_camera(const plane& A, std::size_t res) {	
	pose ps;
	ps.position = Eigen::Vector3f::Zero();
	ps.orientation.setFromTwoVectors(A.normal, Eigen::Vector3f::UnitZ());
	return projection_image_camera(ps, projection_bounding_box::symmetric_orthogonal(6, 6), res, res);
}


int seed = 1;

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


class histogram {
private:
	const float minimum_;
	const float maximum_;
	const float bin_size_;
	std::vector<float> bins_;

public:
	histogram(float mn, float mx, float sz) :
		minimum_(mn), maximum_(mx), bin_size_(sz), bins_(number_of_bins(), 0) { }
		
	std::size_t number_of_bins() const {
		return std::ceil((maximum_ - minimum_) / bin_size_);
	}
		
	std::ptrdiff_t bin_for_value(float val) const {
		return std::floor((val - minimum_) / bin_size_);
	}
	
	float value_for_bin(std::ptrdiff_t i) const {
		return minimum_ + (i * bin_size_);
	}
	
	void add(float val) {
		std::ptrdiff_t i = bin_for_value(val);
		if(i >= 0 && i < bins_.size()) bins_[i] += 1.0;
	}
	
	void normalize() {
		float s = sum();
		for(float& v : bins_) v /= s;
	}
	
	float operator[](std::ptrdiff_t i) const {
		if(i < 0 || i >= bins_.size()) return 0.0;
		else return bins_[i];
	}
	
	float sum(std::ptrdiff_t mn = 0, std::ptrdiff_t mx = -1) const {
		if(mx == -1) mx = bins_.size();
		float s = 0.0;
		for(std::ptrdiff_t i = mn; i < mx; ++i) s += bins_[i];
		return s;
	}
	
	float entropy(std::ptrdiff_t mn = 0, std::ptrdiff_t mx = -1) const {
		if(mx == -1) mx = bins_.size();
		float e = 0;
		float s = sum(mn, mx);
		if(s == 0.0) return 0.0;
		for(std::ptrdiff_t i = mn; i < mx; ++i) {
			if(bins_[i] == 0.0) continue;
			float v = bins_[i] / s;
			e -= v * std::log2(v);
		}
		return e;
	}
	
	std::ptrdiff_t maximal_entropy_threshold() const {
		std::ptrdiff_t i_max;
		float e_max = -INFINITY;
		for(std::ptrdiff_t i = 0; i < bins_.size(); ++i) {
			float e = entropy(0, i) + entropy(i);
			if(e > e_max) {
				e_max = e;
				i_max = i;
			}
		}
		return i_max;
	}
	
	float difference(const histogram& h, float offset = 0.0) const {
		float sz = std::min(bin_size_, h.bin_size_);
		float mn = std::min(minimum_, h.minimum_ + offset);
		float mx = std::max(maximum_, h.maximum_ + offset);
		
		float diff = 0.0;
		for(float v = mn; v < mx; v += sz) {
			std::ptrdiff_t i1 = bin_for_value(v);
			std::ptrdiff_t i2 = h.bin_for_value(v + offset);
			float d = (*this)[i1] - h[i2];
			diff += d*d;
		}
		
		return diff;
	}
	
	void export_to_file(const std::string& file) const {
		std::ofstream out(file);
		for(std::ptrdiff_t i = 0; i < bins_.size(); ++i) {
			out << value_for_bin(i) << ' ' << bins_[i] << std::endl;
		}
	}
};


histogram signed_distance_weights(point_cloud_full& P, const plane& A, float d, float sz) {
	histogram H(-d, +d, sz);
	for(auto& p : P) {
		float d = signed_distance(p, A);
		H.add(d);
		p.set_weight(d);
	}
	H.normalize();
	return H;
}


int main(int argc, const char* argv[]) {
	if(argc >= 2) seed = std::atoi(argv[1]);

	std::cout << "Loading point clouds" << std::endl;
	auto P = hi_point_cloud();
	auto Q = lo_point_cloud();
	
	make_view(P, Q).export_to_image_file("output/initial.png");

	std::cout << "Fitting plane A" << std::endl;
	plane A(0,1,0,0);
	{
		//auto sum = P + Q;
		//A = fit_plane_to_points(sum.begin(), sum.end());
	}
	
	std::cout << "Fitting planes A_P and A_Q" << std::endl;
	plane A_P, A_Q;
	A_P = plane::from_string("-0.988378,-0.118857,-0.0947734,0.460469"); //fit_plane_to_points(P.begin(), P.end());
	A_Q = plane::from_string("-0.972168,-0.0736805,-0.222396,0.363365"); //fit_plane_to_points(Q.begin(), Q.end());
	
	project(P, orthogonal_projection_camera(A_P, 2000)).to_range_image(true).export_visualization_to_image_file("output/A_P.png");
	project(Q, orthogonal_projection_camera(A_Q, 2000)).to_range_image(true).export_visualization_to_image_file("output/A_Q.png");

	project(P, orthogonal_projection_camera(A_P, 2000)).fill_holes().export_visualization_to_image_file("output/A_P_filled.png");
	project(Q, orthogonal_projection_camera(A_Q, 2000)).fill_holes().export_visualization_to_image_file("output/A_Q_filled.png");
	
	std::cout << "Moving orthogonally to planes" << std::endl;
	
	
	const float D = 1.5;
	histogram H_P = signed_distance_weights(P, A_P, D, 0.01);
	histogram H_Q = signed_distance_weights(Q, A_Q, D, 0.01);
	
	H_P.export_to_file("output/H_P.dat");
	H_Q.export_to_file("output/H_Q.dat");
	
	float max_diff = -INFINITY;
	float max_off;
	for(float off = -D; off < +D; off += 0.01) {
		float diff = H_P.difference(H_Q, off);
		if(diff > max_diff) {
			max_diff = diff;
			max_off = off;
		}
	}

	{
		std::cout << max_off << std::endl;
		float t = H_P.value_for_bin(H_P.maximal_entropy_threshold());
		colorize_by_weight(P.begin(), P.end(), t - 0.3, t + 0.3, rgb_color::red, rgb_color::yellow);
		colorize_by_weight(Q.begin(), Q.end(), t + max_off - 0.3, t + max_off + 0.3, rgb_color::red, rgb_color::yellow);
	}

	std::cout << "Moving onto A" << std::endl;
	{
		P.transform( Quaternionf::FromTwoVectors(A.normal, A_P.normal) );
		Q.transform( Quaternionf::FromTwoVectors(A.normal, A_Q.normal) );
	}
	
	make_view(P, Q).export_to_image_file("output/moved.png");
	

	std::cout << "Making projection images" << std::endl;
	auto cam = orthogonal_projection_camera(A, 1000);
	project(P, cam).to_color_image().export_to_image_file("output/P.png");
	project(Q, cam).to_color_image().export_to_image_file("output/Q.png");
}
