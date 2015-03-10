#include <cstdlib>
#include <iostream>
#include <pcf/core.h>
#include <pcf/experiment.h>

using namespace pcf;
using namespace pcf::exper;

int main(int argc, const char* argv[]) {
	auto pc = import_point_cloud("data/hi.ply");
	//auto pc = import_point_cloud("../misc/ply/dragon.ply");
	experiment e(pc);
	e.displacer = constant_displacer(
		pose::from_string("0.0246075,-0.00288276,0.290512,0.00955955,0.184315,0.000947282,0.98282")
	);
	e.loose_modifier = [&](auto& loose, float arg) {
		loose.downsample_random(arg);
	};
	e.create_registration = [&](const auto& fixed, const auto& loose, float arg) {
		return create_iterative_closest_point_registration(fixed, loose, probability_point_filter(0.001));
	};
	e.loose_modifier_runs = 20;
	e.registration_runs = 3;
	e.run("stat.db");
	return EXIT_SUCCESS;
}