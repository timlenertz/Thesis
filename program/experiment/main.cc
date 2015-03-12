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
		loose.downsample_random(1.0 - arg);
	};
	e.loose_modifier_runs = 10;
	
	e.displacer = [](float arg) -> pose {
		return pose().random_displacement(arg * 2.0);
	};
	e.displacer_runs = 5;
	
	e.create_registration = [&](const auto& fixed, const auto& loose, float arg) {
		auto r = create_iterative_closest_point_registration(fixed, loose, probability_point_filter(0.4));
		r->stop_on_divergence = true;
		r->divergence_error_threshold = 0.1;
		r->minimal_error = 0.001;
		r->maximal_iterations = 30;
		return r;
	};
	e.registration_runs = 3;
	
	
	e.create_snapshot = [&](const auto& fixed, const auto& loose) -> color_image {
		projection_image_camera cam(
			pose::from_string("6.30351,0.963818,2.56483,0.541782,0.0358841,0.838688,-0.04228"),
			projection_frustum::symmetric_perspective_fov_x(angle::degrees(90.0), 3.0/2.0),
			300, 200
		);
		camera_range_point_cloud_full<projection_image_camera> rpc(cam);
		rpc.project(fixed);
		rpc.project(loose, rgb_color::red);
		return rpc.to_color_image(rgb_color::black);
	};
	
	e.run_parallel = false;
	
	e.run(db_name);
}

int main(int argc, const char* argv[]) {
	hi_lo("data/hi.ply", "hilo.db");
	return EXIT_SUCCESS;
}