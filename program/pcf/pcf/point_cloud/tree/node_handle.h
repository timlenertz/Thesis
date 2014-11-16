#ifndef PCF_TREE_POINT_CLOUD_NODE_HANDLE_H_
#define PCF_TREE_POINT_CLOUD_NODE_HANDLE_H_

#include <type_traits>
#include <stack>
#include <algorithm>
#include <iostream>
#include <cmath>
#include "tree_point_cloud.h"

namespace pcf {

/**
Handle to node in a tree.
Meant to exist temporarily, contains transient node attributes (bounding box and depth), that
are not stored with the tree but recomputed upon access. Depending on template parameter Const,
gives const or non-const access to the node and its points.
*/
template<typename Traits, typename Point, typename Allocator> template<bool Const>
class tree_point_cloud<Traits, Point, Allocator>::node_handle_ {
public:
	using node_type = typename std::conditional<Const, const node, node>::type;
	using point_type = typename std::conditional<Const, const Point, Point>::type;
	using segment_type = typename std::conditional<Const, const segment, segment>::type;
	
private:
	node_type* nd_;
	bounding_box box_;
	std::ptrdiff_t depth_;
	
public:
	using iterator = typename std::conditional<Const, typename segment_type::const_iterator, typename segment_type::iterator>::type;

	node_handle_() = default;
	node_handle_(const node_handle_<false>& n) :
		nd_(n.nd_), box_(n.box_), depth_(n.depth_) { }
	
	node_handle_(node_type& nd, const bounding_box& cub, std::ptrdiff_t d = -1) :
		nd_(&nd), box_(cub), depth_(d) { }
		
	bool operator==(const node_handle_& n) const { return (nd_ == n.nd_); }
	bool operator!=(const node_handle_& n) const { return (nd_ != n.nd_); }
	bool operator<(const node_handle_& n) const { return (nd_ < n.nd_); }
	
	node_type& operator*() const { return *nd_; }
	node_type* operator->() const { return nd_; }

	const bounding_box& box() const { return box_; }
	std::ptrdiff_t depth() const { return depth_; }
	node_type& attr() const { return *nd_; }
	segment_type& seg() const { return nd_->seg; }

	std::size_t size() const { return nd_->seg.size(); }
	iterator begin() const { return nd_->seg.begin(); }
	iterator end() const { return nd_->seg.end(); }
	
	bool has_child(std::ptrdiff_t i) const;
	bool is_leaf() const;
	node_handle_ child(std::ptrdiff_t i) const;
	
	void make_child(std::ptrdiff_t i, const segment& seg) const;
	
	template<typename Other_point>
	iterator closest_point(const Other_point&, float accepting_distance = 0) const;
};

}

#include "node_handle.tcc"

#endif
