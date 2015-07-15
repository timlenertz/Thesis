auto fx = bunny();
auto lo = fx;
set_unique_color(lo.begin(), lo.end(), rgb_color::red);
set_unique_color(fx.begin(), fx.end(), rgb_color::white);

fx.move_x(0.1);
fx.rotate_z_axis(0.4*pi);
fx.apply_pose();
fx.rotate_z_axis(-0.4*pi, Eigen::Vector3f(0.1, 0.0, 0.0));
fx.move_x(-0.1);


fx.move(-0.001, -0.002, 0.003);
fx.rotate_x_axis(-0.06 * pi);

lo.move_y(0.2);
lo.rotate_z_axis(0.7*pi);
lo.apply_pose();
lo.rotate_z_axis(-0.7*pi, Eigen::Vector3f(0.0, 0.2, 0.0));
lo.move_y(-0.2);

lo.move(0.001, 0.002, -0.003);
lo.rotate_x_axis(0.04 * pi);

kdtree_point_cloud_full lo_r = lo;

viewer_window vw;
vw->add(fx, lo_r);
vw->camera().set_relative_pose(pose::from_string("0.0298457,0.235577,-0.209215,0.120958,-0.0610413,0.95505,0.263673"));
vw.movement_speed = 0.001;

auto reg = make_iterative_closest_point_registration(lo_r, fx, probability_point_filter(0.1));
reg.maximal_iterations = -1;
reg.run_live(lo_r, fx);
