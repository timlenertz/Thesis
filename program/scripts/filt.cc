using namespace Eigen;

grid_point_cloud_full lo_g = hdv_lo();
auto hi = hdv_hi();

compute_normals(lo_g);
orient_normals_to_point(lo_g.begin(), lo_g.end(), Vector3f::Zero(), false);

unorganized_point_cloud_full lo = std::move(lo_g);

viewer_window vw;
vw->camera().set_relative_pose(pose::from_string("4.2147,1.99909,0.763614,-0.116265,0.620531,0.139388,0.762886"));
vw->add(hi);
auto& slo = vw->add(lo);
slo.point_size = 3;
vw->set_background_color(rgb_color::white);
auto& shi = vw->add(hi);
vw.select_object(hi);

ss.filter([](const point_full& p)->bool {
	Vector3f hi_cam = hi.transformation_to(ss) * Vector3f::Zero();
	Vector3f c = (hi_cam - p.coordinates()).normalized();
	Vector3f n = p.get_normal();
	float a = std::acos(n.dot(c));
	return (a < 0.2*pi);
});
ss.handle_update();

for(auto&& p:ss)p.revalidate()
ss.handle_update();


lo.handle_update();
