#include <cstdlib>
#include <iostream>
#include <random>
#include <pcf/core.h>
#include <pcf/experiment.h>

using namespace pcf;
using namespace pcf::exper;

static void hi_lo(const std::string& pc_name, const std::string& db_name) {
	auto pc = import_point_cloud(pc_name);
	experiment e(pc);
	
	const float portion_for_fixed = 0.7;
	std::vector<bool> fixed_points_mask;
	
	e.fixed_modifier = [&](auto& fixed, float arg) {
		fixed.downsample_random(portion_for_fixed);
		fixed_points_mask = fixed.valid_points_mask();
	};
	e.fixed_modifier_runs = 1;
	
	e.loose_modifier = [&](auto& loose, float arg) {
		std::cout << "loose from " << loose.number_of_valid_points() << "..";
		// Take out the points that are in fixed
		loose.filter_mask(fixed_points_mask, true);
		// Additionally downsample to amount arg of points
		loose.downsample_random(0.9995*arg);
		std::cout << "to " << loose.number_of_valid_points() << "!" << std::endl;
	};
	e.loose_modifier_runs = 10;
	
	e.displacer = [](float arg) -> pose {
		return pose().random_displacement(0.03);
	};
	e.displacer_runs = 1;
	
	e.create_registration = [&](const auto& fixed, const auto& loose, float arg) {
		auto r = create_iterative_closest_point_registration(fixed, loose, probability_point_filter(0.001));
		r->stop_on_divergence = true;
		r->divergence_error_threshold = 0.1;
		r->minimal_error = 0.0001;
		r->maximal_iterations = 20;
		return r;
	};
	e.registration_runs = 1;
	
	e.minimal_actual_error = 0.0001;
	
	int i = 1;
	e.run_callback = [&i](const run_result& run) {
		run.export_animation("output/v"+std::to_string(i++)+".mov", "jpeg");
	};
	
	
	e.create_snapshot = [&](const auto& fixed, const auto& loose, const Eigen::Affine3f& transformation) -> color_image {
		projection_image_camera cam(
			pose::from_string("6.30351,0.963818,2.56483,-0.016081,0.617942,0.0424625,0.784912"),
			projection_frustum::symmetric_perspective_fov_x(angle::degrees(60.0), 3.0/2.0),
			600, 400
		);
		camera_range_point_cloud_full<projection_image_camera> rpc(cam);
		unorganized_point_cloud_xyz loose_t(loose);
		loose_t.set_relative_pose(loose.relative_pose());
		loose_t.transform(transformation);
		rpc.project(fixed, rgb_color::red);
		rpc.project(loose_t);
		return rpc.to_color_image(rgb_color::black);
	};
	
	e.run_parallel = false;
	
	results res = e.run(db_name);
}

int main(int argc, const char* argv[]) {
	hi_lo("data/hi.ply", "output/hilo.db");
	return EXIT_SUCCESS;
}