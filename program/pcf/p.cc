#include <iostream>

using namespace pcf;

point_cloud_xyz bunny_model() {
	ply_reader ply("../bunny.ply");
	return point_cloud_xyz::create_from_reader(ply);
}


int main(int argc, const char* argv[]) {
	range_point_cloud_xyz pc(bunny_model(), );
}