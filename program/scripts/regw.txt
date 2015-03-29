unorganized_point_cloud_full s1 = make_sphere_point_cloud(1.0, 300000);
auto s2 = s1;
s2.move_x(2.5);
set_unique_color(s2.begin(), s2.end(), rgb_color::red);
set_unique_weight(s1.begin(), s1.end(), 0.0);
set_unique_weight(s2.begin(), s2.end(), 1.0);
auto pc1 = s1 + s2;
pc1.move_center_of_mass_to_origin();
pc1.apply_pose();
normalize_point_weights(pc1.begin(), pc1.end());


s2.move_x(0.2);
set_unique_color(s2.begin(), s2.end(), 0.7*rgb_color::red);
set_unique_color(s1.begin(), s1.end(), 0.7*rgb_color::white);
set_unique_weight(s1.begin(), s1.end(), 1.0);
set_unique_weight(s2.begin(), s2.end(), 0.0);
auto pc2 = s1 + s2;
pc2.move_center_of_mass_to_origin();
pc2.apply_pose();
normalize_point_weights(pc2.begin(), pc2.end());

pose cam_ps = pose::from_string("0,0,4.58752,1,0,0,0");
projection_image_camera cam(cam_ps, projection_frustum::symmetric_perspective_fov_x(angle::degrees(60), 3.0/2.0), 600, 400);
pc1.downsample_projection(cam);
pc2.downsample_projection(cam);

kdtree_point_cloud_full pc1t(pc1);

pose ps;
ps.random_displacement(0.02, 0.1*pi);
pc1t.set_relative_pose(ps);

viewer_window vw;
vw->add(pc1t, pc2);
vw->camera().set_relative_pose(pose::from_string("0,0,4.71461,1,0,0,0"));
vw.select_object(pc1t);


auto corw = [](const point_xyz& a, const point_full& b)->float {
	if(distance(a, b) > 0.4) return 0.0;
	else return b.get_weight();
};
auto pc2_r = pc2;
auto reg = make_iterative_closest_point_registration(pc1t, pc2_r, accept_point_filter(), corw)

reg.run_live(pc1t, pc2, pc2_r);


space_registration_correspondences_list lst;
lst.set_parent(pc1t);
reg.correspondences(*lst);
vw->add(lst);
