#include <pcf/core.h>
#include <pcf/experiment.h>

using namespace pcf;
using namespace pcf::exper;


int main() {
	experiment e;
	
	relief r(5.0, 1, 0.5);
	
	auto point_clouds = split_random_downsampled(import_point_cloud("../misc/ply/bunny.ply"), 0.5);
	
	e.make_fixed = [&](float arg) -> experiment::working_point_cloud_type {
		return r.make_point_cloud(30000);
	};
	e.make_fixed_runs = 1;
	
	e.make_loose = [&](const experiment::fixed_point_cloud_type&, float arg) -> experiment::working_point_cloud_type {
		return r.make_point_cloud(30000 - 10 + arg*arg*20000);
	};
	e.make_loose_runs = 10;
		
	e.displacer = [](float arg) -> pose {
		return pose::random_displacement(0.01, angle::degrees(15.0*arg + 3.0));
	};
	e.displacer_runs = 1;
	
	e.create_registration = [&](const auto& fixed, const auto& loose, float arg) {
		auto r = create_iterative_closest_point_registration(fixed, loose, accept_point_filter());
		r->stop_on_divergence = false;
		r->divergence_error_threshold = 0.1;
		r->minimal_error = 0;
		r->maximal_iterations = 40;
		return r;
	};
	e.registration_runs = 1;
	
	e.minimal_actual_error = 0;
	
	e.run_parallel = true;
	
	e.run_callback = [](const run_result& run, std::ptrdiff_t i) {
		run.export_animation("output/v"+std::to_string(i++)+".mov", "mp4v");
	};
	
	
	e.create_snapshot = [&](const auto& fixed, const auto& loose, const Eigen::Affine3f& transformation) -> rgb_color_image {
		projection_image_camera cam(
			pose::from_string("0.0523251,-4.45682,4.36287,0.93091,0.365195,0.00584923,-0.00229461"),
			projection_frustum::symmetric_perspective_fov_x(angle::degrees(60.0), 2.0/3.0),
			600, 400
		);
		camera_range_point_cloud_full<projection_image_camera> rpc(cam);
		unorganized_point_cloud_xyz loose_t(loose);
		loose_t.set_relative_pose(loose.relative_pose());
		loose_t.transform(transformation);
		rpc.project(fixed, rgb_color::red);
		rpc.project(loose_t);
		rgb_color_image img = rpc.to_rgb_color_image(rgb_color::black);
		img.flip(true, false);
		return img;
	};
	

	results res = e.run("output/relief_hilo.db");
}
