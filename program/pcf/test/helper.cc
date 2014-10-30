#include "helper.h"
#include "../src/io/ply_reader.h"

namespace pcf {

point_cloud<point_xyz> bunny_model() {
	ply_reader ply("../bunny.ply");
	return point_cloud<point_xyz>::create_from_reader(ply);
}

}