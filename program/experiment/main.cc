#include <cstdlib>
#include <iostream>

#include <pcf/core.h>
#include <pcf/viewer.h>

using namespace pcf;
using namespace Eigen;

range_image frontal_view(const point_cloud_full& pc, const plane& A) {	
	pose ps;
	ps.position = A.project(pc.center_of_mass());
	ps.orientation.setFromTwoVectors(A.normal, Eigen::Vector3f::UnitZ());
	projection_image_camera cam(ps, projection_bounding_box::symmetric_orthogonal(6, 6), 600, 600);
	
	return project(pc, cam).to_range_image();
}


auto correspondences(const kdtree_point_cloud_full& P, const kdtree_point_cloud_full& Q) {
	return make_closest_point_correspondences(P, Q, probability_point_filter(0.001));
}

struct cor_receiver {
	float a, b, c;
	std::vector<std::array<float, 2>> D;
	std::vector<float> y;
	
	cor_receiver& operator<<(const registration_correspondence& rc) {
		const Vector3f& p = rc.fixed;
		const Vector3f& q = rc.loose;
		D.push_back({ b*q[2] - c*q[1], c*q[0] - a*q[2] });
		y.push_back( -a*q[0] - b*q[1] - c*q[2] + a*p[0] + b*p[1] + c*p[2] );
		return *this;
	}
	
	cor_receiver& operator<<(const cor_receiver& rec) {
		y.insert(y.end(), rec.y.begin(), rec.y.end());
		D.insert(D.end(), rec.D.begin(), rec.D.end());
		return *this;
	}

};

auto hi_point_cloud() {
	return make_relief_point_cloud(5.0, 5000, 0);
	//grid_point_cloud_full gpc = import_point_cloud("data/hi.ply");
	//compute_normals(gpc);
	//return gpc;
}

auto lo_point_cloud() {
	unorganized_point_cloud_full upc = make_relief_point_cloud(5.0, 5000, 0);
	//grid_point_cloud_full gpc = import_point_cloud("data/lo.ply");
	//compute_normals(gpc);
	//unorganized_point_cloud_full upc = std::move(gpc);
	upc.random_displacement(0.5, angle::degrees(20));
	upc.apply_pose();
	return upc;
}

int main(int argc, const char* argv[]) {
	std::cout << "Loading point clouds" << std::endl;
	kdtree_point_cloud_full hi = hi_point_cloud();
	kdtree_point_cloud_full lo = lo_point_cloud();

	std::cout << "Fitting plane" << std::endl;
	plane A = fit_plane_to_points(hi.begin(), hi.end());
	Matrix3f RA;
	{
		Quaternionf quat;
		quat.setFromTwoVectors(A.normal, Vector3f::UnitZ());
		RA = quat.toRotationMatrix();
	}
	std::cout << "n = \n" << A.normal << std::endl;
	std::cout << "RA = \n" << RA << std::endl;


	std::cout << "Finding point correspondences" << std::endl;	
	MatrixXf D;
	VectorXf y;
	{
		auto cor = correspondences(hi, lo);
		cor_receiver rec;
		rec.a = RA(0, 2);
		rec.b = RA(1, 2);
		rec.c = RA(2, 2);
		cor(rec, Affine3f::Identity());
		
		std::size_t n = rec.D.size();
		D = MatrixXf(n, 3);
		y = VectorXf(n);
		
		for(std::ptrdiff_t i = 0; i != n; ++i) {
			D(i, 0) = rec.D[i][0];
			D(i, 1) = rec.D[i][1];
			D(i, 2) = 1;
			y[i] = rec.y[i];
		}
	}
	
	std::cout << "Calculating estimated transformation" << std::endl;
	Vector3f x = (D.transpose() * D).inverse() * D.transpose() * y;
	
	frontal_view(hi, A).export_visualization_to_image_file("hi.png");
	frontal_view(lo, A).export_visualization_to_image_file("lo.png");
	
	std::cout << "Applying estimated transformation" << std::endl;
	{
		Affine3f R(AngleAxisf(x[0], Vector3f::UnitX()) * AngleAxisf(x[1], Vector3f::UnitY()));
		Vector3f t(0, 0, x[2]);
		Translation3f T(t);
		
		Affine3f ps = lo.relative_pose().transformation_to_world();
		ps = RA.inverse() * (T * R).inverse() * RA * ps;
		lo.set_relative_pose(ps);
	}
	
	std::cout << x << std::endl;
	
	frontal_view(lo, A).export_visualization_to_image_file("lo2.png");
}
