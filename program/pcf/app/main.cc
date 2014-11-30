#include "glfw.h"
#include <cstdlib>
#include <thread>
#include <memory>
#include "viewer_window.h"
#include "../pcf/point_cloud/unorganized_point_cloud.h"
#include "../pcf/point_algorithm.h"
#include "../pcf/io/ply_importer.h"
#include "../pcf/io/pointscan_importer.h"
#include "../pcf_viewer/scene/bounding_box.h"

using namespace pcf;

std::unique_ptr<viewer_window> viewer_win = nullptr;

unorganized_point_cloud_full load_pointscan(const char* filename) {
	pointscan_importer ps(filename);
	unorganized_point_cloud_full pc(ps);
	return pc;
}

unorganized_point_cloud_xyz load_ply(const char* filename) {
	ply_importer ps(filename);
	unorganized_point_cloud_xyz pc(ps);
	return pc;
}

void user_interface() {
	viewer_win->access_scene([&](scene& sc) {
		//sc.add_point_cloud( load_pointscan("../../../townhall/Scan_042_2.scan") );
		
		unorganized_point_cloud_xyz dragon = load_ply("../../data/dragon.ply");
		
		auto& pc = sc.add_point_cloud(dragon);
		set_unique_color(pc->begin(), pc->end(), rgb_color::green);
		
		auto& bb = sc.add_bounding_box(dragon.box());
		bb.set_color(rgb_color::green);

		Eigen::Affine3f t ( Eigen::AngleAxisf(0.1*M_PI, Eigen::Vector3f::UnitX()) );
		dragon.apply_transformation(t);
		sc.add_point_cloud(dragon);

		sc.add_bounding_box(dragon.box());

		std::cout << "added objects" << std::endl;
	});
	
	for(;;) {
		std::cout << "input" << std::endl;
		int i;
		std::cin >> i;
		std::cout << i;
	}
}

int main(int argc, const char* argv[]) {
	glfwInit();
	viewer_win.reset( new viewer_window() );

	std::thread ui_thread(user_interface);

	viewer_win->run();	
	ui_thread.detach();

	std::atexit(glfwTerminate);
	
	return EXIT_SUCCESS;
}