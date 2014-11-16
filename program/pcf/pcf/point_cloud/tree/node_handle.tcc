#include <array>
#include <cassert>
#include <algorithm>
#include <cmath>
#include "../../point_algorithm.h"

namespace pcf {

template<typename Traits, typename Point, typename Allocator> template<bool Const>
bool tree_point_cloud<Traits, Point, Allocator>::node_handle_<Const>::
has_child(std::ptrdiff_t i) const {
	return bool(nd_->children[i]);
}

template<typename Traits, typename Point, typename Allocator> template<bool Const>
bool tree_point_cloud<Traits, Point, Allocator>::node_handle_<Const>::
is_leaf() const {
	for(const auto& c : nd_->children) if(c) return false;
	return true;
}


template<typename Traits, typename Point, typename Allocator> template<bool Const>
auto tree_point_cloud<Traits, Point, Allocator>::node_handle_<Const>::
child(std::ptrdiff_t i) const -> node_handle_ {
	assert(has_child(i));
	return node_handle_(
		*(nd_->children[i]),
		Traits::child_box(i, box_, attr(), depth_),
		depth_ + 1
	);
}


template<typename Traits, typename Point, typename Allocator> template<bool Const>
void tree_point_cloud<Traits, Point, Allocator>::node_handle_<Const>::
make_child(std::ptrdiff_t i, const segment& seg) const {
	nd_->children[i].reset( new node(seg) );
}


	
template<typename Traits, typename Point, typename Allocator> template<bool Const> template<typename Other_point>
auto tree_point_cloud<Traits, Point, Allocator>::node_handle_<Const>::
closest_point(const Other_point& query, float accepting_distance) const -> iterator {
	// If this is a leaf, search in its points
	if(is_leaf()) return find_closest_point(query, begin(), end(), accepting_distance);
	
	if(Traits::number_of_children == 2) {
		// Optimization for 2 children (eg KdTree)
		std::ptrdiff_t ci = Traits::child_box_closer_to_point(query, 0, 1, box_, attr(), depth_);
		std::ptrdiff_t ci2 = 1 - ci;
		
		iterator c_pt = child(ci).closest_point(query);
		float c_d = distance_sq(*c_pt, query);
		if(c_d <= accepting_distance) return c_pt;		

		auto c2 = child(ci2);
		float c2_d = minimal_distance_sq(query, c2.box());
		if(c2_d < c_d) {
			iterator c2_pt = c2.closest_point(query);
			float c2_d = distance_sq(*c2_pt, query);
			if(c2_d < c_d) return c2_pt;
		}
		return c_pt;
	}
	
	// Get child node indices, sorted by ascending minimal distance of the child box to the query point
	std::array<std::ptrdiff_t, Traits::number_of_children> closest_children;
	for(std::ptrdiff_t i = 0; i < Traits::number_of_children; ++i) closest_children[i] = i;
	std::sort(closest_children.begin(), closest_children.end(), [this, &query](std::ptrdiff_t a, std::ptrdiff_t b) {
		return Traits::child_box_closer_to_point(query, a, b, box_, attr(), depth_);
	});
	
	// Get closest existing child node
	auto ci = closest_children.cbegin();
	while(! has_child(*ci)) ++ci;
	
	// Recursively: take closest point within that child
	iterator pt = child(*ci).closest_point(query);
	float d = distance_sq(*pt, query);
		
	// Accept current point if distance <= accepting_distance
	if(d <= accepting_distance) return pt;		

	// A closer point may be in the other children
	for(; ci != closest_children.cend(); ++ci) {	
		if(! has_child(*ci)) continue;
		
		// Get minimal distance to box of child c
		node_handle_ c = child(*ci);
		float min_c_d = minimal_distance_sq(query, c.box());
		
		// When it is greater than distance to pt, child can contain no closer point
		// Stop because subsequent children will have even larger min_c_d
		if(min_c_d > d) break;
		
		// If it is smaller than d, a closer point may be in c
		iterator c_pt = c.closest_point(query);
		float c_d = distance_sq(*c_pt, query);
		if(c_d < d) {
			d = c_d;
			pt = c_pt;
			if(d <= accepting_distance) return pt;	
		}
	}
	
	return pt;
}


}