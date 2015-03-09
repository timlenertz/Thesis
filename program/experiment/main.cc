#include <cstdlib>
#include <iostream>
#include <pcf/core.h>
#include <pcf/experiment.h>

int main(int argc, const char* argv[]) {
	using namespace pcf;
	auto pc = import_point_cloud("../misc/ply/bunny.ply");
	experiment exper(pc);
	exper.displacer = default_displacement();
	exper.create_registration = [](const experiment::fixed_point_cloud_type& fixed, const experiment::loose_point_cloud_type& loose) {
		return create_iterative_closest_point_registration(fixed, loose);
	};
	exper.displacer_runs = 10;
	exper.run();
	return EXIT_SUCCESS;
}