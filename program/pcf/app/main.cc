#include <GLFW/glfw3.h>
#include <cstdlib>
#include <thread>
#include <memory>
#include "viewer_window.h"
#include "../pcf/point_cloud/unorganized_point_cloud.h"
#include "../pcf/io/ply_importer.h"

using namespace pcf;

std::unique_ptr<viewer_window> viewer_win = nullptr;

unorganized_point_cloud_xyz load(const char* filename) {
	ply_importer ply(filename);
	unorganized_point_cloud_xyz pc(ply);
	return pc;
}

void user_interface() {
	viewer_win->access_scene([&](scene& sc) {
		sc.add_point_cloud( load("../../data/dragon.ply") );
		std::cout << "added pc" << std::endl;
	});
	
	int i;
	std::cin >> i;
	std::cout << i;
}

int main(int argc, const char* argv[]) {
	glfwInit();
	viewer_win.reset( new viewer_window() );

	std::thread ui_thread(user_interface);
	viewer_win->run();	

	std::atexit(glfwTerminate);
	
	return EXIT_SUCCESS;
}