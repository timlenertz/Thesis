#ifndef PCF_KDREE_TRAITS_H_
#define PCF_KDREE_TRAITS_H_

#include <array>
#include "../../geometry/bounding_box.h"
#include "../segment.h"
#include "tree_point_cloud.h"

namespace pcf {

/**
Traits for KdTree structure.
Split plane gets chosen at median position in each node.
*/
class kdtree_traits {
public:
	const static std::size_t number_of_children = 2;
	const static std::size_t maximal_depth = -1;

	struct node_attributes {
		float split_plane;
	};
	
	static bounding_box child_box(std::ptrdiff_t i, const bounding_box&, const node_attributes&, std::ptrdiff_t depth);

	template<typename Point>
	static bounding_box root_box(point_cloud_segment<Point>);
	
	template<typename Other_point>
	static std::ptrdiff_t child_containing_point(const Other_point& p, const bounding_box&, const node_attributes&, std::ptrdiff_t depth);

	template<typename Other_point>
	static std::ptrdiff_t child_box_closest_to_point(const Other_point& p, const bounding_box&, const node_attributes&, std::ptrdiff_t depth);

	template<typename Other_point>
	static std::ptrdiff_t child_box_closer_to_point(const Other_point& p, std::ptrdiff_t a, std::ptrdiff_t b, const bounding_box&, const node_attributes&, std::ptrdiff_t depth);
	
	template<typename Point>
	static std::array<point_cloud_segment<Point>, 2> split_node(point_cloud_segment<Point>, const bounding_box&, node_attributes&, std::ptrdiff_t depth);
	
	template<typename Point>
	static node_attributes initialize_node_attributes
	(point_cloud_segment<Point>, const bounding_box&, std::ptrdiff_t depth);
};

using kdtree_point_cloud_xyz = tree_point_cloud<kdtree_traits, point_xyz>;
using kdtree_point_cloud_full = tree_point_cloud<kdtree_traits, point_full>;

PCF_DECLARE_TREE_POINT_CLOUD_INSTANTIATION(kdtree_traits);

}

#include "kdtree_traits.tcc"

#endif