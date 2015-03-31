using namespace Eigen;

pose ps1 = pose::from_string("0.0111054,-5.34705,3.16065,0.888177,0.459471,0.00140555,0.00514437");
pose ps2 = pose::from_string("1.19905,5.00239,3.73934,0.0271785,0.0955291,0.430536,0.897092");
pose vps = pose::from_string("-3.91371,0.746101,4.11871,0.631202,0.178814,-0.261022,-0.70815");

range_image_camera cam1(ps1, angle::degrees(60), angle::degrees(60), 600, 600);
range_image_camera cam2(ps2, angle::degrees(60), angle::degrees(60), 600, 600);

auto og = make_relief_point_cloud(5.0, 100000);
og.set_point_indices();

auto fx_1 = og, lo_1 = og;
set_unique_color(lo_1.begin(), lo_1.end(), rgb_color::red);
set_unique_color(fx_1.begin(), fx_1.end(), rgb_color::blue);
fx_1.downsample_projection_depth(cam1, 0.4);
lo_1.downsample_projection_depth(cam2, 0.4);


for(point_full& lo_p : lo_1) {
	const point_full& same_fx_p = *(fx_1.begin() + lo_p.get_index());	
	if(! same_fx_p.valid()) lo_p.set_color(rgb_color::black);
}
for(point_full& fx_p : fx_1) {
	const point_full& same_lo_p = *(lo_1.begin() + fx_p.get_index());	
	if(! same_lo_p.valid()) fx_p.set_color(rgb_color::black);
}

kdtree_point_cloud_full fx = fx_1;
kdtree_point_cloud_full lo = lo_1;

lo.random_displacement(0.2, angle::degrees(2.0));

viewer_window vw;
auto& sfx = vw->add(fx);
auto& slo = vw->add(lo);
vw.select_object(lo);
vw->camera().set_relative_pose(vps);

auto select_func = [&](const point_full& lo_p) {
	const point_full& same_fx_p = *(fx_1.begin() + lo_p.get_index());	
	return same_fx_p.valid() && (random_integer(0, 10) < 2);
};

auto weight_func = [&](const point_full& fx_p, const point_full& lo_p) -> float {
	return 1.0;
};

space_registration_correspondences_list lst;
lst.set_parent(fx);
vw->add(lst);

auto reg = make_iterative_closest_point_registration(fx, lo, select_func, weight_func);

auto reg_step = [&](bool vis = true) {
	//reg_step_pre(vis);

	reg.apply_loose_transformation(fx, lo);

	reg.iteration();
	if(vis) {
		lst->clear();
		reg.correspondences(*lst, reg.accumulated_transformation());
		lst.handle_update();
	}

	std::cout << "Error: " << reg.current_error() << std::endl;
};


reg_step_pre(false);
reg.run_live(fx, lo, [&]{ reg_step_pre(false); });


