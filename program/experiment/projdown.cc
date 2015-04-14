#include <random>
#include "projdown.h"

using namespace pcf;
using namespace pcf::exper;


void projdown(int seed, const std::string& db_name) {
	float width = 5.0;
	float full_density = 20000;

	experiment e;
	
	unorganized_point_cloud_full og = make_relief_point_cloud(width, full_density, seed);
	
	e.make_fixed = [&](float arg) -> experiment::working_point_cloud_type {
		const std::size_t img_sz = 5000;
		const float rad = 4.0, z = 3.0;
		const float a = arg * two_pi;
		auto pc = og;
		Eigen::Vector3f rel_pos(std::cos(a)*rad, std::sin(a)*rad, z);
		pose ps;
		ps.position = rel_pos;
		projection_image_camera cam(
			ps,
			projection_frustum::symmetric_perspective_fov_x(60.0, 1.0),
			img_sz, img_sz
		);
		cam.look_at(pc);
		pc.downsample_projection(cam);
		return pc;
	};
	e.make_fixed_runs = 10;
	
	e.make_loose = [&](const experiment::fixed_point_cloud_type&, float arg) -> experiment::working_point_cloud_type {	
		auto pc = og;
		const std::size_t im_sz = 50 + (1.0-arg)*500;
		projection_image_camera cam(
			pose::from_string("0.220013,-4.92678,3.35604,0.912652,0.408613,0.00823912,0.00583399"),
			projection_frustum::symmetric_perspective_fov_x(angle::degrees(60.0), 1.0),
			im_sz, im_sz
		);
		pc.downsample_projection(cam);
		return pc;
	};
	e.make_loose_runs = 1;
		
	e.displacer = [](float arg) -> pose {
		return pose::random_displacement(0.5*arg + 0.1, angle::degrees(15.0*arg + 3.0));
	};
	e.displacer_runs = 4;
	
	e.create_registration = [&](const auto& fixed, const auto& loose, float arg) {
		auto r = create_iterative_closest_point_registration(fixed, loose, accept_point_filter());
		r->stop_on_divergence = false;
		r->divergence_error_threshold = 0.1;
		r->minimal_error = 0.000001;
		r->maximal_iterations = 50;
		return r;
	};
	e.registration_runs = 3;
	
	e.minimal_actual_error = 0.0001;
	
	e.run_callback = [](const run_result& run, std::ptrdiff_t i) {
		run.export_animation("output/v"+std::to_string(i++)+".mov", "mp4v");
	};
	
	
	e.create_snapshot = [&](const auto& fixed, const auto& loose, const Eigen::Affine3f& transformation) -> color_image {
		projection_image_camera cam(
			pose::from_string("0,0,5,1,0,0,0"),
			projection_frustum::symmetric_perspective_fov_x(angle::degrees(60.0), 1.0),
			1000, 1000
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
