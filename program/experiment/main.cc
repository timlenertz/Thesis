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

static void r() {
	using namespace Eigen;

	pose ps1 = pose::from_string("0.0111054,-5.34705,3.16065,0.888177,0.459471,0.00140555,0.00514437");
	pose ps2 = pose::from_string("1.19905,5.00239,3.73934,0.0271785,0.0955291,0.430536,0.897092");
	pose vps = pose::from_string("-3.91371,0.746101,4.11871,0.631202,0.178814,-0.261022,-0.70815");

	range_image_camera cam1(ps1, angle::degrees(60), angle::degrees(60), 600, 600);
	range_image_camera cam2(ps2, angle::degrees(60), angle::degrees(60), 600, 600);

	auto og = make_relief_point_cloud(5.0, 100000);

	auto fx = project(og, cam1);
	auto lo = project(og, cam2);
	set_unique_color(lo.begin(), lo.end(), rgb_color::red);

	lo.random_displacement(0.2);

	auto weight_func = [&](const point_full& fx_p, const point_full& lo_p) -> float {
		Vector3f fx_n = fx_p.get_normal();
		Vector3f lo_n = lo_p.get_normal();

		Vector3f lo_cam = lo.transformation_to(fx) * Eigen::Vector3f::Zero();
		Vector3f fx_cam = Vector3f::Zero();

		Vector3f fx_to_ls_cam = (fx_p.coordinates() - lo_cam).normalized();
		Vector3f lo_to_fx_cam = (lo_p.coordinates() - fx_cam).normalized();
	
		float fx_visibility = -fx_to_ls_cam.dot(fx_n);
		if(fx_visibility < 0) fx_visibility = 0;

		float lo_visibility = -lo_to_fx_cam.dot(lo_n);
		if(lo_visibility < 0) lo_visibility = 0;

		return fx_visibility * lo_visibility;
	};

	kdtree_point_cloud_full fxt = fx;

	auto reg = make_iterative_closest_point_registration(fxt, lo, accept_point_filter(), weight_func);
	//reg.run_live(fxt, lo);
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

	return EXIT_SUCCESS;
}