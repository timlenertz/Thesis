#ifndef PCF_TREE_POINT_CLOUD_H_
#define PCF_TREE_POINT_CLOUD_H_

#include <memory>
#include <array>
#include <stack>
#include <vector>
#include "point_cloud.h"

namespace pcf {

template<typename Traits, typename Point, typename Allocator = aligned_allocator<Point>>
class tree_point_cloud : public point_cloud<Point, Allocator> {
	using super = point_cloud<Point, Allocator>;
	using segment = typename super::segment;
	using node_attributes = typename Traits::node_attributes;

private:
	struct node : node_attributes {
		segment seg;
		std::array<std::unique_ptr<node>, Traits::number_of_children> children;
		
		node() = default;
		node(const segment& seg) : seg(seg) { }
	};

public:
	template<typename> class node_handle_;
	using node_handle = node_handle_<node>;
	using const_node_handle = node_handle_<const node>;
	using backtrace = typename node_handle::backtrace;
	using const_backtrace = typename const_node_handle::backtrace;

private:	
	const std::size_t leaf_capacity_;
	node root_node_;
	cuboid root_cuboid_;

	void build_tree_();
	bool verify_(const const_node_handle&) const;

public:
	template<typename Other_cloud> tree_point_cloud(Other_cloud&& pc, std::size_t leaf_cap, const Allocator&);
	template<typename Other_cloud> tree_point_cloud(Other_cloud&& pc, std::size_t leaf_cap);

	node_handle root();
	const_node_handle root() const;

	bool verify() const { return verify_(root()); }
};



template<typename Traits, typename Point, typename Allocator> template<typename Node>
class tree_point_cloud<Traits, Point, Allocator>::node_handle_ {
	static_assert(
		std::is_same<
			typename std::remove_const<Node>::type,
			node
		>::value,
		"Template argument Node must be 'node' or 'const node'."
	);
	
private:
	Node* nd_;
	cuboid cub_;
	std::ptrdiff_t depth_;
	
public:
	using backtrace = std::vector<node_handle_>;

	template<typename Inserter>
	void locality_(std::size_t k, const backtrace&, Inserter) const;

	node_handle_() = default;
	node_handle_(const node_handle_<typename std::remove_const<Node>::type>& n) :
		nd_(n.nd_), cub_(n.cub_), depth_(n.depth_) { }
	
	node_handle_(Node& nd, const cuboid& cub, std::ptrdiff_t d = -1) :
		nd_(&nd), cub_(cub), depth_(d) { }
		
	bool operator==(const node_handle_& n) const { return (nd_ == n.nd_); }
	bool operator!=(const node_handle_& n) const { return (nd_ != n.nd_); }
	bool operator<(const node_handle_& n) const { return (nd_ < n.nd_); }
	
	Node& operator*() const { return *nd_; }
	Node* operator->() const { return nd_; }

	const cuboid& cub() const { return cub_; }
	std::ptrdiff_t depth() const { return depth_; }
	Node& attr() const { return *nd_; }
	Node& nd() const { return *nd_; }

	std::size_t size() const { return seg().size(); }
	
	bool has_child(std::ptrdiff_t i) const;
	bool is_leaf() const;
	node_handle_ child(std::ptrdiff_t i) const;
	
	void make_child(std::ptrdiff_t i, const segment& seg) const;
	
	template<typename Other_point> std::ptrdiff_t child_containing_point(const Other_point&) const;
	
	template<typename Other_point> node_handle_ deepest_child_containing_point(const Other_point&, std::ptrdiff_t max_depth = -1) const;	
	template<typename Other_point> node_handle_& deepest_child_containing_point(const Other_point&, backtrace&, std::ptrdiff_t max_depth = -1) const;	
	
	template<typename Callback_func, typename Order_func> bool depth_first_descent(Callback_func callback, Order_func order) const;

	template<typename Callback_func, typename Order_func> void visit_neightboring_nodes(Callback_func callback, Order_func order, const backtrace& bt) const;
	template<typename Callback_func, typename Order_func> void visit_neightboring_leaves(Callback_func callback, Order_func order, const backtrace& bt) const;

};

}

#include "tree_point_cloud_node_handle.tcc"
#include "tree_point_cloud.tcc"

#endif
