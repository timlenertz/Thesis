#include <pcf/core.h>
#include <pcf/experiment.h>
#include <iostream>
#include <fstream>

using namespace pcf;
using namespace pcf::exper;

int main() {
	kdtree_point_cloud_full p = import_point_cloud("data/hi.ply");
	kdtree_point_cloud_full q = import_point_cloud("data/lo.ply");
	auto cor_q_to_p = make_closest_point_correspondences(p, q, accept_point_filter());
	auto tester_mae = make_error_metric_range_tester(cor_q_to_p, mean_absolute_error());
	float mag_t = 0.2;
	angle mag_r = angle::degrees(6.0);
	tester_mae.test_random_ranges_1dim("plot.dat", mag_t, mag_r, 10, 30);

}
/*
template<typename Tester>
void batch(const Tester& tester, const std::string& dir, const std::vector<std::pair<std::string, Eigen::Affine3f>>& tests) {
	for(const auto& p : tests) {
		const std::string& fname = dir + p.first;
		std::cout << "doing " << fname << "..." << std::endl;
		tester.test_range_1dim(fname, p.second);
		std::cout << "done " << fname << std::endl;
	}
}

int main() {
	std::cout << "creating point clouds..." << std::endl;
	
	
	auto cam_p = parallel_projection_camera_at_angle(5.0, angle::degrees(110.0), 3.0, 256);
	//auto cam_p = parallel_projection_camera_top_down(5.0, 10.0, 512);
	
	auto cam_q = parallel_projection_camera_at_angle(5.0, angle::degrees(10.0), 3.0, 3111);
	//auto cam_q = parallel_projection_camera_at_angle(5.0, angle::degrees(100), 10.0, 100);

	relief r(5.0, 1, 1.0);
	auto qr = r.make_projected_point_cloud(cam_q, 500, true);
	auto pr = r.make_projected_point_cloud(cam_p, 500, true);

	unorganized_point_cloud_full p = pr, q = qr;
	p.apply_pose(); q.apply_pose();
	
	kdtree_point_cloud_full kp = p, kq = q;
	
	// Correspondences
	float d_max = 0.2;
	auto cond = [&](const point_full& p_, const point_full& q_) -> bool {
		float dot = p_.get_normal().dot(q_.get_normal());
		return (distance(p_, q_) < d_max) && (std::abs(dot) > std::cos(angle::degrees(20)));
	};
	auto cor_q_to_p = make_closest_point_correspondences(kp, q, accept_point_filter(), cond);
	cor_q_to_p.rejecting_distance = d_max;
	auto cor_p_to_q = make_closest_point_correspondences(kq, p, accept_point_filter(), cond);
	cor_p_to_q.rejecting_distance = d_max;

	std::string dir = "output/t3rr/";
	std::cout << "starting..." << std::endl;

	// True transformation error
	auto scor = make_same_point_correspondences(p, q);	
	scor.mode = decltype(scor)::known_transformation;
	auto tester_known = make_error_metric_range_tester(scor, mean_absolute_error());

	// Histogram error
	float p_l = 10.0 / 256;
	
	bool rej = true;
	
	std::size_t it = 10;
	std::size_t sc = 4;
	float mag_t = 0;
	angle mag_r = 0;
	std::string fname;
		
	cross_histogram_error err_chi(p_l, cam_p.relative_pose(), 100, cross_histogram_error::chi_squared, rej);

	
	//cross_histogram_error err_cor(p_l, cam_p.relative_pose(), 100, cross_histogram_error::correlation, rej);
	//cross_histogram_error err_int(p_l, cam_p.relative_pose(), 100, cross_histogram_error::intersection, rej);

	auto tester_mae = make_error_metric_range_tester(cor_p_to_q, mean_absolute_error());
	auto tester_chi = make_error_metric_range_tester(cor_q_to_p, err_chi);
/*
	//mag_t = 0.1;
	mag_r = angle::radiants(0.04);
	fname = "L";
	
	std::cout << "chi " << fname << std::endl;
	tester_chi.test_random_ranges_1dim(dir + fname + "_chi.dat", mag_t, mag_r, sc, it);
	std::cout << "mae " << fname << std::endl;
	tester_mae.test_random_ranges_1dim(dir + fname + "_mae.dat", mag_t, mag_r, sc, it);


	//mag_t = 0.01;
	mag_r = angle::degrees(0.004);
	fname = "M";
	
	std::cout << "chi " << fname << std::endl;
	tester_chi.test_random_ranges_1dim(dir + fname + "_chi.dat", mag_t, mag_r, sc, it);
	std::cout << "mae " << fname << std::endl;
	tester_mae.test_random_ranges_1dim(dir + fname + "_mae.dat", mag_t, mag_r, sc, it);




	it = 100;
	sc = 4;

	//mag_t = 0.001;
	mag_r = angle::degrees(0.0004);
	fname = "S";

	//tester_chi.test_random_ranges_1dim(dir + fname + "_chi.dat", mag_t, mag_r, sc, it);
	
	kp.rotate_x_axis(angle::degrees(0.0001));
	kp.rotate_y_axis(angle::degrees(0.0002));
	kp.rotate_z_axis(angle::degrees(0.0003));
	tester_chi.test_random_ranges_1dim(dir + fname + "_chi_o1.dat", mag_t, mag_r, sc, it);
	
	//std::cout << "chi " << fname << std::endl;
	//tester_chi.test_random_ranges_1dim(dir + fname + "_chi.dat", mag_t, mag_r, sc, it);
	//std::cout << "mae " << fname << std::endl;
	//tester_mae.test_random_ranges_1dim(dir + fname + "_mae.dat", mag_t, mag_r, sc, it);
}


		
	std::cout << "cor" << std::endl;
	make_error_metric_range_tester(cor_q_to_p, err_cor).test_random_ranges_1dim
		(dir + fname + "_cor.dat", mag_t, mag_r, 4, 100);
		
	std::cout << "int" << std::endl;
	make_error_metric_range_tester(cor_q_to_p, err_int).test_random_ranges_1dim
		(dir + fname + "_int.dat", mag_t, mag_r, 4, 100);

*/