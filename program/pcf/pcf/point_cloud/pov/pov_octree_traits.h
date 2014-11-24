#ifndef PCF_POV_OCTREE_TRAITS_H_
#define PCF_POV_OCTREE_TRAITS_H_

#include "../tree/octree_traits.h"

namespace pcf {

class pov_octree_traits : public octree_traits {
	using super = octree_traits;

public:
	struct node_attributes : super::node_attributes {
		Eigen::Vector3f center_of_mass;
	};

	template<typename Point>
	static std::array<point_cloud_segment<Point>, 8> split_node
	(point_cloud_segment<Point> seg, const bounding_box& box, node_attributes& attr, std::ptrdiff_t depth) {
		attr.center_of_mass = seg.center_of_mass();
		return super::split_node(seg, box, attr, depth);
	}
};

}

#endif