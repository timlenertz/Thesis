#ifndef PCF_OCTREE_TRAITS_H_
#define PCF_OCTREE_TRAITS_H_

#include <array>
#include "../../geometry/cuboid.h"
#include "../point_cloud_segment.h"

namespace pcf {

class octree_traits {
public:
	static constexpr std::size_t number_of_children = 8;
	static constexpr std::size_t maximal_depth = -1;
	
	struct node_attributes { };
	
	static cuboid child_cuboid(std::ptrdiff_t i, const cuboid&, const node_attributes&, std::ptrdiff_t depth);

	template<typename Point>
	static cuboid root_cuboid(point_cloud_segment<Point>);
	
	template<typename Other_point>
	static std::ptrdiff_t child_containing_point(const Other_point& p, const cuboid&, const node_attributes&, std::ptrdiff_t depth);
	
	template<typename Point>
	static std::array<point_cloud_segment<Point>, 8> split_node(point_cloud_segment<Point>, const cuboid&, node_attributes&, std::ptrdiff_t depth);
};

}

#include "octree_traits.tcc"

#endif