unorganized_point_cloud_full lo = bunny();
kdtree_point_cloud_full hi = lo;
set_unique_color(lo.begin(), lo.end(), rgb_color::white);
set_unique_color(hi.begin(), hi.end(), rgb_color::red);

viewer_window vw;
vw->add(hi, lo);
vw.select_object(lo);
vw.movement_speed = 0.001;

lo.move(0.001, 0.002, -0.003);
lo.rotate_x_axis(0.05 * pi)

auto reg = make_iterative_closest_point_registration(hi, lo, accept_point_filter());
space_registration_correspondences_list lst;
lst.set_parent(hi);
reg.correspondences(*lst);
vw->add(lst);
