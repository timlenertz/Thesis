#include "glfw.h"
#include <cstdlib>
#include <thread>
#include <memory>
#include "viewer_window.h"
#include "shell.h"
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

unorganized_point_cloud_full load_ply(const char* filename) {
	ply_importer ps(filename);
	unorganized_point_cloud_xyz pc(ps);
	return pc;
}

void user_interface() {
	viewer_win->access_scene([&](scene& sc) {
		shell::get().main();
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