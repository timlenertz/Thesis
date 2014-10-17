#ifndef PCF_KDTREE_POINT_CLOUD_H_
#define PCF_KDTREE_POINT_CLOUD_H_

#include <utility>
#include <functional>
#include <memory>
#include <array>
#include <cstdint>
#include "segmented_point_cloud.h"
#include "../geometry/cuboid.h"

namespace pcf {

template<typename Point, typename Allocator = std::allocator<Point>>
class kdtree_point_cloud : public segmented_point_cloud<Point, Allocator> {
	using super = segmented_point_cloud<Point, Allocator>;
	using typename super::segment;
	
private:
	class node;
		
	struct node_cuboid : cuboid {	
		std::uint8_t orientation = 0; // 0=x, 1=y, 2_z, modulo
	
		node_cuboid child_cuboid(bool left, const node& nd) const;
		template<typename Other_point> bool child_for_point(const Other_point&, const node& nd) const;
		
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
	kdtree_point_cloud(const Other_cloud& pc, std::size_t leaf_cap, const Allocator& alloc = Allocator());
	
	void verify() const { verify_(root_node_, root_cuboid_); }
	
	template<typename Other_point, typename Distance_func>
	const Point& find_closest_point(const Other_point& from, Distance_func dist) const;
};




template<typename Point, typename Allocator>
class kdtree_point_cloud<Point, Allocator>::node : public segment {
	using super = segment;
	
private:
	std::unique_ptr<node> left_, right_;
	float split_plane_ = NAN;
		
public:
	using super::super;
	explicit node(const segment& seg) :
		super(seg) { }
	
	node(const node&) = delete;
	node& operator=(const node&) = delete;
	
	bool is_leaf() const;

	node& left() { assert(! is_leaf()); return *left_; }
	node& right() { assert(! is_leaf()); return *right_; }
	const node& left() const { assert(! is_leaf()); return *left_; }
	const node& right() const { assert(! is_leaf()); return *right_; }
	
	float split_plane() const { return split_plane_; }

	void create_children(Point* split, const node_cuboid& cub);
};



}

#include "kdtree_point_cloud.tcc"

#endif