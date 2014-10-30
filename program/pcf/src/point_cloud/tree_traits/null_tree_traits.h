#ifndef PCF_NULL_TREE_TRAITS_H_
#define PCF_NULL_TREE_TRAITS_H_

#include <array>
#include "../../geometry/bounding_box.h"
#include "../point_cloud_segment.h"

namespace pcf {

class null_tree_traits {
public:
	const static std::size_t number_of_children = 1;
	const static std::size_t maximal_depth = 0;

	struct node_attributes { };
	
	static bounding_box child_box(std::ptrdiff_t, const bounding_box& cub, const node_attributes&, std::ptrdiff_t) {
		return cub;
	}

	template<typename Point>
	static bounding_box root_box(point_cloud_segment<Point> seg) {
		return seg.box(1);
	}
	
	template<typename Other_point>
	static std::ptrdiff_t child_containing_point(const Other_point& p, const bounding_box&, const node_attributes&, std::ptrdiff_t) {
		return 0;
	}
	
	template<typename Point>
	static std::array<point_cloud_segment<Point>, 1> split_node(point_cloud_segment<Point> seg, const bounding_box&, node_attributes&, std::ptrdiff_t) {
		return { seg };
	}
};

}

#endif