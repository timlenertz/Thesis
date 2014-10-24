#ifndef PCF_NULL_TREE_TRAITS_H_
#define PCF_NULL_TREE_TRAITS_H_

#include <array>
#include "../../geometry/cuboid.h"
#include "../point_cloud_segment.h"

namespace pcf {

class null_tree_traits {
public:
	static constexpr std::size_t number_of_children = 1;
	static constexpr std::size_t maximal_depth = 0;

	struct node_attributes { };
	
	static cuboid child_cuboid(std::ptrdiff_t, const cuboid& cub, const node_attributes&, std::ptrdiff_t) {
		return cub;
	}

	template<typename Point>
	static cuboid root_cuboid(point_cloud_segment<Point> seg) {
		return seg.bounding_cuboid(1);
	}
	
	template<typename Other_point>
	static std::ptrdiff_t child_containing_point(const Other_point& p, const cuboid&, const node_attributes&, std::ptrdiff_t) {
		return 0;
	}
	
	template<typename Point>
	static std::array<point_cloud_segment<Point>, 1> split_node(point_cloud_segment<Point> seg, const cuboid&, node_attributes&, std::ptrdiff_t) {
		return { seg };
	}
};

}

#endif