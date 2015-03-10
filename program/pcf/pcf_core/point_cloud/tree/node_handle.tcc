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
auto tree_point_cloud<Traits, Point, Allocator>::node_handle_<Const>::
closest_point(const point_xyz& query, float accepting_distance, float rejecting_distance) const -> iterator {	
	// Return end() if this node's box is outside rejection radius.
	if(rejecting_distance != INFINITY) {
		if(minimal_distance_sq(query, box()) >= rejecting_distance) return end();
	}

	// If this is a leaf, search in its points
	if(is_leaf()) return find_closest_point(query, begin(), end(), accepting_distance);

	iterator pt = end();
	float d = INFINITY;

	// Look in child whose box is closest to query (it it exists)
	std::ptrdiff_t closest_i = Traits::child_box_closest_to_point(query, box_, attr(), depth_);	
	if(has_child(closest_i)) {
		node_handle_ c = child(closest_i);
		pt = c.closest_point(query, accepting_distance, rejecting_distance);

		// If closest_point returned end iterator, box was outside rejection radius.
		if(pt == c.end()) return end();

		d = distance_sq(*pt, query);
		// If d in accepting radius, take this point
		if(d <= accepting_distance) return pt;	
	}
	
	// Need to look in other children for closer/any points...
	// Get other child node indices, sorted by ascending minimal distance of the child box to the query point
	std::array<std::ptrdiff_t, Traits::number_of_children - 1> other_children;
	if(Traits::number_of_children == 2) {
		// Just one other child in binary tree
		other_children[0] = 1 - closest_i;
	} else {
		// Sort remaining children by ascending minimal point-to-box distance (slow)
		std::ptrdiff_t cur = 0;
		for(std::ptrdiff_t& c : other_children) {
			if(cur == closest_i) ++cur;
			c = cur++; 
		}
		std::sort(other_children.begin(), other_children.end(), [this, &query](std::ptrdiff_t a, std::ptrdiff_t b) {
			return Traits::child_box_closer_to_point(query, a, b, box_, attr(), depth_);
		});
	}
	
	for(std::ptrdiff_t other_i : other_children) {	
		if(! has_child(other_i)) continue;
		
		// Get minimal distance to box of child c
		node_handle_ c = child(other_i);
		float min_c_d = minimal_distance_sq(query, c.box());
		
		// When it is greater than distance to pt, child can contain no closer point
		// Stop because subsequent children will have even larger min_c_d
		if(min_c_d > d) break;
		
		// If it is smaller than d, a closer point may be in c
		iterator c_pt = c.closest_point(query, accepting_distance, rejecting_distance);
		
		// If closest_point returned end iterator, box was outside rejection radius
		if(c_pt == c.end()) break;
		
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