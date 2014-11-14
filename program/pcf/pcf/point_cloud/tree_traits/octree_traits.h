#ifndef PCF_OCTREE_TRAITS_H_
#define PCF_OCTREE_TRAITS_H_

#include <array>
#include "../../geometry/bounding_box.h"
#include "../segment.h"

namespace pcf {

class octree_traits {
public:
	const static std::size_t number_of_children = 8;
	const static std::size_t maximal_depth = -1;
	
	struct node_attributes { };
	
	static bounding_box child_box(std::ptrdiff_t i, const bounding_box&, const node_attributes&, std::ptrdiff_t depth);

	template<typename Point>
	static bounding_box root_box(point_cloud_segment<Point>);
	
	template<typename Other_point>
	static std::ptrdiff_t child_containing_point(const Other_point& p, const bounding_box&, const node_attributes&, std::ptrdiff_t depth);
	
	template<typename Point>
	static std::array<point_cloud_segment<Point>, 8> split_node(point_cloud_segment<Point>, const bounding_box&, node_attributes&, std::ptrdiff_t depth);
};

}

#include "octree_traits.tcc"

#endif