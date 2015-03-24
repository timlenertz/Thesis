#include <cstdlib>
#include <iostream>

#include "hilo.h"
#include "projdown.h"

using namespace pcf;

pcf::range_point_cloud_full hdv_scan(const std::string& id) {
	std::string path = "../hdv/Scan_" + id + ".scan";
	auto pc = pcf::import_range_point_cloud(path);
	pc.set_name(id);
	return pc;
}

pcf::range_point_cloud_full hdv_scan_cut(const std::string& id, const std::string& bb, const std::string& ps) {
	using namespace pcf;
	auto pc = hdv_scan(id);
	pc.filter( crop_point_filter<bounding_box>(bounding_box::from_string(bb)) );
	pc.set_relative_pose( pose::from_string(ps) );
	return pc;
}

pcf::range_point_cloud_full hdv_lo() {
	return hdv_scan_cut(
		"005",
		"-17.6605,-5.41663,1.72615,-14.3355,-2.02658,6.63569",
		"16.764,3.82095,-4.20347,1,0,0,0"
	);
}


pcf::range_point_cloud_full hdv_hi() {
	return hdv_scan_cut(
		"012",
		"-7.11634,-5.53676,-2.2105,-5.63491,-2.51254,1.64889",
		"5.78633,4.91419,1.24274,0.995499,-0.00072259,-0.0845465,0.0428211"
	);
}


int main(int argc, const char* argv[]) {
	/*hilo(
		"data/hi.ply",
		"output/hilo.db",
		pose::from_string("4.15846,0.876933,2.20932,-0.0618967,0.561422,0.0666719,0.822514")
	);*/
	
	/*projdown(
		"data/hi.ply",
		"output/projdown.db",
		pose::from_string("4.15846,0.876933,2.20932,-0.0618967,0.561422,0.0666719,0.822514"),
		4.0
	);*/
	
	grid_point_cloud_full gpc(hdv_lo());
	set_local_density_weights(gpc, 20);
	
	projection_image_camera cam(
		pose::from_string("4.86971,-0.0192937,1.28612,0.00660215,0.604592,-0.013184,0.796399"),
		projection_frustum::symmetric_perspective_fov_x(angle::degrees(60.0), 12.0/16.0),
		1600, 1200
	);
	auto rpc = project(gpc, cam);
	rpc.project(gpc);
	auto img = rpc.weights_to_intensity_image();
	img.flip(true, false);
	//img.export_to_image_file("a.png");
	img.export_visualization_to_image_file("a.png");

	return EXIT_SUCCESS;
}