#include <random>
#include <cmath>
#include "projdown.h"

using namespace pcf;
using namespace pcf::exper;

static pose do_projection_downsampling(experiment::working_point_cloud_type& pc, const pose& cam_pose, angle a, float rad) {
	const std::size_t img_sz = 500;
	Eigen::Vector3f rel_pos(std::cos(a)*rad, std::sin(a)*rad, 0.0);
	pose ps;
	ps.position = cam_pose.transform_to_world(rel_pos);
	projection_image_camera cam(
		ps,
		projection_frustum::symmetric_perspective_fov_x(20.0, 1.0),
		img_sz, img_sz
	);
	cam.look_at(pc);
	
	pc.downsample_projection(cam);
	
	return cam.relative_pose();
}

void projdown(const std::string& pc_name, const std::string& db_name, const pose& cam_pose, float circle_radius) {
	auto pc = import_point_cloud(pc_name);
	experiment e(pc);
	
	const float portion_for_fixed = 0.5;
	std::vector<bool> fixed_points_mask;
	
	pose downsample_cam_pose;
	
	e.fixed_modifier = [&](auto& fixed, float arg) {
		fixed.downsample_random(portion_for_fixed);
		downsample_cam_pose = do_projection_downsampling(fixed, cam_pose, arg * two_pi, circle_radius);
	};
	e.fixed_modifier_runs = 10;
	
	
	e.loose_modifier = [&](auto& loose, float arg) {
		loose.filter_mask(fixed_points_mask, true);
		do_projection_downsampling(loose, cam_pose, arg * two_pi, circle_radius);
	};
	e.loose_modifier_runs = 10;
	
	e.displacer = [](float arg) -> pose {
		pose ps;
		ps.random_displacement(0.1, 0.03*pi);
		return ps;
	};
	e.displacer_runs = 3;
	
	e.create_registration = [&](const auto& fixed, const auto& loose, float arg) {
		auto r = create_iterative_closest_point_registration(fixed, loose, accept_point_filter());
		r->stop_on_divergence = true;
		r->divergence_error_threshold = 0.1;
		r->minimal_error = 0.001;
		r->maximal_iterations = 40;
		return r;
	};
	e.registration_runs = 1;
	
	e.minimal_actual_error = 0.0005;
	
	e.run_callback = [](const run_result& run, std::ptrdiff_t i) {
		run.export_animation("output/v"+std::to_string(i++)+".mov", "mp4v");
	};
	
	
	e.create_snapshot = [&](const auto& fixed, const auto& loose, const Eigen::Affine3f& transformation) -> color_image {
		projection_image_camera cam(
			cam_pose,
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
