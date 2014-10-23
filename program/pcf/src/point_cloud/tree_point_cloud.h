#ifndef PCF_TREE_POINT_CLOUD_H_
#define PCF_TREE_POINT_CLOUD_H_

#include <memory>
#include <array>
#include "point_cloud.h"

namespace pcf {

template<typename Traits, typename Point, typename Allocator = std::allocator<Point>>
class tree_point_cloud : public point_cloud<Point, Allocator> {
	using super = point_cloud<Point, Allocator>;
	using typename super::segment;
	using node_attributes = typename Traits::node_attributes;

private:
	struct node : node_attributes {
		segment seg;
		std::array<std::unique_ptr<node>, Traits::number_of_children> children;
		
		node() = default;
		node(const segment& seg) : seg(seg) { }
	};
	
	class node_handle;
	
	const std::size_t leaf_capacity_;
	node root_node_;
	std::unique_ptr<node_handle> root_handle_;

	node_handle& root_() { return *root_node_; }
	const node_handle& root_() const { return *root_node_; }

	void build_tree_();
	bool verify_(const node_handle&) const;

	auto node_locality_(std::size_t k, const node_handle&) const;
	
	template<typename Callback_func, typename Order_func>
	void depth_first_descent_(Callback_func callback, Order_func order, const node_handle&);

	template<typename Callback_func, typename Order_func>
	void breadth_first_descent_(Callback_func callback, Order_func order, const node_handle&);

public:
	template<typename Other_cloud> tree_point_cloud(Other_cloud&& pc, std::size_t leaf_cap, const Allocator&);
	template<typename Other_cloud> tree_point_cloud(Other_cloud&& pc, std::size_t leaf_cap);

	bool verify() const { return verify_(*root_handle_); }
};



template<typename Traits, typename Point, typename Allocator>
class tree_point_cloud<Traits, Point, Allocator>::node_handle {
private:
	node* nd_;
	cuboid cub_;
	std::ptrdiff_t depth_;

public:
	node_handle(node& nd, const cuboid& cub, std::ptrdiff_t d = -1) :
		nd_(&nd), cub_(cub), depth_(d) { }
	
	node& operator*() { return *nd_; }
	const node& operator*() const { return *nd_; }
	node* operator->() { return nd_; }
	const node* operator->() const { return nd_; }

	const cuboid& cub() const { return cub_; }
	std::ptrdiff_t depth() const { return depth_; }
	node_attributes& attr() { return *nd_; }
	const node_attributes& attr() const { return *nd_; }

	const segment& seg() const { return nd_->seg; }
	segment& seg() { return nd_->seg; }
	
	bool has_child(std::ptrdiff_t i) const;
	bool is_leaf() const;
	const node_handle child(std::ptrdiff_t i) const;
	node_handle child(std::ptrdiff_t i);
	
	void make_child(std::ptrdiff_t i, const segment& seg);
};


}

#include "tree_point_cloud.tcc"

#endif
