auto pc = import_point_cloud("data/lo.ply");
ofstream str("lo.dat")
for(std::size_t w = 10; w <= 3500; w += 20) {
	projection_image_camera cam(plane::from_string("-0.972168,-0.0736805,-0.222396,0.363365"), 6, 6, w);
	auto rpc = project(pc, cam);
	float fill = rpc.ratio_of_area_filled();
	float cont = (float)rpc.number_of_valid_points() / pc.size();
	str << w << " " << fill << " " << cont << endl;
}
str.close();


hi: -0.988378,-0.118857,-0.0947734,0.460469
lo: -0.972168,-0.0736805,-0.222396,0.363365


auto pc = import_point_cloud("data/lo.ply");
auto cam = projection_image_camera(plane::from_string("-0.972168,-0.0736805,-0.222396,0.363365"), 6, 6, 1);
auto cam2 = set_optimal_camera_image_size(cam, pc);
cout << cam2.image_width() << endl;


auto pc = import_point_cloud("data/hi.ply");
auto cam = projection_image_camera(plane::from_string("-0.988378,-0.118857,-0.0947734,0.460469"), 6, 6, 1);
auto cam2 = set_optimal_camera_image_size(cam, pc);
cout << cam2.image_width() << endl;
