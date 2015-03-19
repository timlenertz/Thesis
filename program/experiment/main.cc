#include <cstdlib>
#include <iostream>

#include "hilo.h"
#include "projdown.h"

using namespace pcf;

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
	
	auto pc = import_point_cloud("../misc/ply/bunny.ply");
	grid_point_cloud_full gpc(pc);
	set_local_density_weights(gpc);
	
	ply_exporter exp("normal.ply");
	gpc.export_with(exp);
	
	return EXIT_SUCCESS;
}