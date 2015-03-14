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
		// Take out the points that are in fixed
		loose.filter_mask(fixed_points_mask, true);
		// Additionally downsample to amount arg of points
		loose.downsample_grid(0.08*arg + 0.01);
	};
	e.loose_modifier_runs = 15;
	
	e.displacer = [](float arg) -> pose {
		return pose().random_displacement(1.0*arg + 0.1, (0.1*arg + 0.03)*pi);
	};
	e.displacer_runs = 5;
	
	e.create_registration = [&](const auto& fixed, const auto& loose, float arg) {
		auto r = create_iterative_closest_point_registration(fixed, loose);
		r->stop_on_divergence = true;
		r->divergence_error_threshold = 0.1;
		r->minimal_error = 0.00005;
		r->maximal_iterations = 100;
		return r;
	};
	e.registration_runs = 5;
	
	e.minimal_actual_error = 0.0005;
	
	e.run_callback = [](const run_result& run, std::ptrdiff_t i) {
		run.export_animation("output/v"+std::to_string(i++)+".mov", "mp4v");
	};
	
	
	e.create_snapshot = [&](const auto& fixed, const auto& loose, const Eigen::Affine3f& transformation) -> color_image {
		projection_image_camera cam(
			pose::from_string("4.15846,0.876933,2.20932,-0.0618967,0.561422,0.0666719,0.822514"),
			projection_frustum::symmetric_perspective_fov_x(angle::degrees(60.0), 3.0/2.0),
			1200, 800
		);
		camera_range_point_cloud_full<projection_image_camera> rpc(cam);
		unorganized_point_cloud_xyz loose_t(loose);
		loose_t.set_relative_pose(loose.relative_pose());
		loose_t.transform(transformation);
		rpc.project(fixed, rgb_color::red);
		rpc.project(loose_t);
		color_image img = rpc.to_color_image(rgb_color::black);
		img.flip(true, false);
		return img;
	};
	
	e.run_parallel = true;
	
	results res = e.run(db_name);
}

int main(int argc, const char* argv[]) {
	hi_lo("data/hi.ply", "output/hilo.db");
	
	return EXIT_SUCCESS;
}