using namespace Eigen;

kdtree_point_cloud_full hi = import_point_cloud("data/hi.ply");
unorganized_point_cloud_full lo = import_point_cloud("data/lo.ply");

viewer_window vw;
vw->add(lo, hi);
vw->camera().set_relative_pose(pose::from_string("4.2147,1.99909,0.763614,-0.116265,0.620531,0.139388,0.762886"));
vw.select_object(lo);
vw->set_background_color(0xffffff);

auto corcond = [](const point_full& a, const point_full& b)->bool {
	return true;
	float d = a.get_normal().dot(b.get_normal());
	float ang = std::acos(d);
	return true;
	return (ang < half_pi);
};

auto corw = [](const point_full& a, const point_full& b)->float {
	if(distance(a, b) > 0.2) return 0.0;
	else return 1.0;
};

auto sel = [](const point_full& p)->bool {
	return true;
};

auto cor = make_closest_point_correspondences(hi, lo, sel, corw, corcond);
cor.rejecting_distance = 0.2;
cor.accepting_distance = 0.01;

iterative_correspondences_registration<decltype(cor)> reg(cor);
reg.maximal_iterations = -1;
reg.run_live(hi, lo);


auto fut = reg.run_live(hi, lo);
fut.get();

