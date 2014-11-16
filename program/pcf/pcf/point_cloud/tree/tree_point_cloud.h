#ifndef PCF_TREE_POINT_CLOUD_H_
#define PCF_TREE_POINT_CLOUD_H_

#include <memory>
#include <array>
#include <stack>
#include <vector>
#include <cmath>
#include "../point_cloud.h"

namespace pcf {

template<typename Traits, typename Point, typename Allocator = aligned_allocator<Point>>
class tree_point_cloud : public point_cloud<Point, Allocator> {
	using super = point_cloud<Point, Allocator>;
	using segment = typename super::segment;
	using const_segment = typename super::const_segment;
	using node_attributes = typename Traits::node_attributes;

private:
	struct node : node_attributes {
		segment seg;
		std::array<std::unique_ptr<node>, Traits::number_of_children> children;
		
		node() = default;
		node(const segment& seg) : seg(seg) { }
	};

public:
	template<bool> class node_handle_;
	using node_handle = node_handle_<false>;
	using const_node_handle = node_handle_<true>;

private:	
	const std::size_t leaf_capacity_;
	node root_node_;
	bounding_box root_box_;

	void build_tree_();
	bool verify_(const const_node_handle&) const;

public:
	template<typename Other_cloud> tree_point_cloud(Other_cloud&& pc, std::size_t leaf_cap, const Allocator&);
	template<typename Other_cloud> tree_point_cloud(Other_cloud&& pc, std::size_t leaf_cap);

	node_handle root();
	const_node_handle root() const;

	bool verify() const { return verify_(root()); }

	template<typename Other_point>	
	const Point& closest_point(const Other_point& query, float accepting_distance = 0, float rejecting_distance = INFINITY) const;
};

}

#include "tree_point_cloud.tcc"

#endif
