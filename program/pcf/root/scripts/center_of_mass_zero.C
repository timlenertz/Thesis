void center_of_mass_zero(const std::string& name) {
	using namespace pcf;
	auto pc = import_point_cloud(name);
	auto c = pc.center_of_mass();
	pose p(Eigen::Affine3f(Eigen::Translation3f(-c)));
	pc.set_relative_pose(p);
	pc.apply_pose();
	ply_exporter xp("n"+name);
	pc.export_with(xp);
}