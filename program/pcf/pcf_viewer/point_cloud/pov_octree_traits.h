#ifndef PCFVW_POV_OCTREE_TRAITS_H_
#define PCFVW_POV_OCTREE_TRAITS_H_

#include "../../pcf_core/point_cloud/tree/octree_traits.h"

namespace pcf {

class pov_octree_traits : public octree_traits {
	using super = octree_traits;

public:
	struct node_attributes : super::node_attributes {
		Eigen::Vector3f center_of_mass;
	};

	template<typename Point>
	static node_attributes initialize_node_attributes
	(point_cloud_segment<Point> seg, const bounding_box& box, std::ptrdiff_t depth) {
		node_attributes attr;
		attr.center_of_mass = seg.center_of_mass();
		return attr;
	}	
};

}

#endif
