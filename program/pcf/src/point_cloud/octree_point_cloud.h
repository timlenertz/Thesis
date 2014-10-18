#ifndef PCF_OCTREE_POINT_CLOUD_H_
#define PCF_OCTREE_POINT_CLOUD_H_

#include <utility>
#include <functional>
#include <memory>
#include <array>
#include "segmented_point_cloud.h"
#include "../geometry/cuboid.h"

namespace pcf {

template<typename Point, typename Allocator = std::allocator<Point>>
class octree_point_cloud : public segmented_point_cloud<Point, Allocator> {
	using super = segmented_point_cloud<Point, Allocator>;
	using typename super::segment;
	
private:
	class node;
		
	struct node_cuboid : cuboid {
		node_cuboid child_cuboid(std::ptrdiff_t) const;
		template<typename Other_point> std::ptrdiff_t child_for_point(const Other_point&) const;
		
		using cuboid::cuboid;
		using cuboid::operator=;
	};
	

	const std::size_t leaf_capacity_;
	node root_node_;
	node_cuboid root_cuboid_;
	
	void build_tree_();
	void split_node_(node&, const node_cuboid&);
	void verify_(const node&, const node_cuboid&) const;
		
	template<typename Other_point>
	const node& node_containing_point_(const Other_point& pt, const node& par_nd, const node_cuboid& par_cub, std::size_t max_depth = 0) const;
	
public:
	template<typename Other_cloud>
	octree_point_cloud(Other_cloud&& pc, std::size_t leaf_cap, const Allocator& alloc = Allocator());
	
	void verify() const { verify_(root_node_, root_cuboid_); }
	
	template<typename Other_point>
	const Point& find_closest_point(const Other_point& from) const;
};




template<typename Point, typename Allocator>
class octree_point_cloud<Point, Allocator>::node : public segment {
	using super = segment;

private:
	using child_ptr = std::unique_ptr<node>;
	std::array<child_ptr, 8> children_;
	
public:
	using super::super;
	explicit node(const segment& seg) : super(seg) { }
	
	node(const node&) = delete;
	node& operator=(const node&) = delete;
	
	bool has_child(std::ptrdiff_t i) const { return bool(children_[i]); }
	bool is_leaf() const;

	node& child(std::ptrdiff_t i) { assert(has_child(i)); return *children_[i]; }
	const node& child(std::ptrdiff_t i) const { assert(has_child(i)); return *children_[i]; }

	node& create_child(std::ptrdiff_t i, const segment& seg);
};



}

#include "octree_point_cloud.tcc"

#endif