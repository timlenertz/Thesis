#include <random>
#include "hilo.h"

using namespace pcf;
using namespace pcf::exper;

void hilo(int seed, const std::string& db_name) {
	float width = 5.0;
	float full_density = 5000;

	experiment e;
	
	e.make_fixed = [&](float arg) -> experiment::working_point_cloud_type {
		return make_relief_point_cloud(width, full_density, seed);
	};
	e.make_fixed_runs = 1;
	
	e.make_loose = [&](const experiment::fixed_point_cloud_type&, float arg) -> experiment::working_point_cloud_type {
		float a = (0.35-0.2)*arg + 0.2; 
		float density = full_density * (1.0 - a)*(1.0 - a);
		return make_relief_point_cloud(width, density, seed);
	};
	e.make_loose_runs = 10;
		
	e.displacer = [](float arg) -> pose {
		return pose::random_displacement(0.5*arg + 0.1, angle::degrees(15.0*arg + 3.0));
	};
	e.displacer_runs = 4;
	
	e.create_registration = [&](const auto& fixed, const auto& loose, float arg) {
		auto r = create_iterative_closest_point_registration(fixed, loose, accept_point_filter());
		r->stop_on_divergence = false;
		r->divergence_error_threshold = 0.1;
		r->minimal_error = 0.000001;
		r->maximal_iterations = 100;
		return r;
	};
	e.registration_runs = 3;
	
	e.minimal_actual_error = 0.0001;
	
	e.run_callback = [](const run_result& run, std::ptrdiff_t i) {
		run.export_animation("output/v"+std::to_string(i++)+".mov", "mp4v");
	};
	
	
	e.create_snapshot = [&](const auto& fixed, const auto& loose, const Eigen::Affine3f& transformation) -> color_image {
		projection_image_camera cam(
			pose::from_string("0.220013,-4.92678,3.35604,0.912652,0.408613,0.00823912,0.00583399"),
			projection_frustum::symmetric_perspective_fov_x(angle::degrees(60.0), 2.0/3.0),
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
