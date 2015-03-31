#include <random>
#include <cmath>
#include "relief.h"

using namespace pcf;
using namespace pcf::exper;
using namespace Eigen;


void relief(const std::string& db_name) {
	using efx_t = experiment::fixed_point_cloud_type;
	using els_t = experiment::loose_point_cloud_type;

	pose ps1 = pose::from_string("0.0111054,-5.34705,3.16065,0.888177,0.459471,0.00140555,0.00514437");
	pose ps2 = pose::from_string("1.19905,5.00239,3.73934,0.0271785,0.0955291,0.430536,0.897092");
	pose vps = pose::from_string("-3.91371,0.746101,4.11871,0.631202,0.178814,-0.261022,-0.70815");

	range_image_camera cam1(ps1, angle::degrees(60), angle::degrees(60), 300, 300);
	range_image_camera cam2(ps2, angle::degrees(60), angle::degrees(60), 300, 300);

	auto og = make_relief_point_cloud(5.0, 100000);
	og.set_point_indices();

	experiment e(og);
	
	e.fixed_modifier = [&](auto& fixed, float arg) {
		fixed.downsample_projection(cam1);
	};

	e.loose_modifier = [&](auto& loose, float arg) {
		loose.downsample_projection(cam2);
	};
	
	e.displacer = [](float arg)->pose {
		return pose::random_displacement(0.2, angle::degrees(10.0));
	};
	e.displacer_runs = 10;
	
	e.create_registration = [&](efx_t& fx, els_t& ls, float arg) {
		efx_t* fx_pc = &fx;
		els_t* ls_pc = &ls;
	
		auto selection_func = [&, fx_pc, ls_pc](const point_full& ls_p) -> bool {
			const point_full& fx_p = *(fx_pc->begin() + ls_p.get_index());
			return fx_p.valid();
		};
	
		auto weight_func = [&](const point_full& fx_p, const point_full& lo_p) -> float {
			return 1.0;
		};


		auto r = create_iterative_closest_point_registration(fx, ls, selection_func, weight_func);
		r->iteration_preprocess = [&, r] {
			/*Eigen::Affine3f loose_to_world = ls.absolute_pose().transformation_to_world();
			loose_to_world = r->accumulated_transformation() * loose_to_world;
			pose ps(loose_to_world);

			compute_normal_direction_weights(fx.begin(), fx.end(), ps.transformation_from_world()*ps2.position, angle::degrees(150.0));
			compute_normal_direction_weights(ls.begin(), ls.end(), ls.absolute_pose().transformation_from_world()*ps1.position, angle::degrees(150.0));

			colorize_by_weight(fx.begin(), fx.end(), 0, 1, rgb_color::blue, rgb_color::green);
			colorize_by_weight(ls.begin(), ls.end(), 0, 1, rgb_color::red, rgb_color::yellow);*/
		};
		r->maximal_iterations = 20;
		return r;
	};
	
	e.run_callback = [](const run_result& run, std::ptrdiff_t i) {
		run.export_animation("output/v"+std::to_string(i++)+".mov", "mp4v");
	};

	e.create_snapshot = [&](const auto& fx, const auto& ls, const Eigen::Affine3f& transformation) -> color_image {
		projection_image_camera cam(
			vps,
			projection_frustum::symmetric_perspective_fov_x(angle::degrees(60.0), 2.0/3.0),
			1200, 800
		);
		camera_range_point_cloud_full<projection_image_camera> rpc(cam);
		unorganized_point_cloud_full ls2(ls);
		ls2.set_relative_pose(ls.relative_pose());
		ls2.transform(transformation);
		
		rpc.project(fx);
		rpc.project(ls2);
		color_image img = rpc.to_color_image(rgb_color::black);
		
		img.flip(true, false);
		return img;
	};

	results res = e.run(db_name);
}