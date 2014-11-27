#include <GLFW/glfw3.h>
#include <cstdlib>
#include "viewer_window.h"
#include <pcf_viewer/scene/scene.h>

unorganized_point_cloud_xyz load(const char* filename) {
	ply_importer ply(argv[1]);
	unorganized_point_cloud_xyz pc(ply);
	return pc;
}

int main(int argc, const char* argv[]) {
	scene sc(800, 600);
	
	sc.add_point_cloud( load(argv[1]) );

	glfwInit();
	viewer_window window;
	glfwTerminate();
	
	return EXIT_SUCCESS;
}