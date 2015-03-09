#ifndef PCF_TREE_POINT_CLOUD_H_
#define PCF_TREE_POINT_CLOUD_H_

#include <memory>
#include <array>
#include <vector>
#include <cmath>
#include "../point_cloud.h"


#define PCF_DECLARE_TREE_POINT_CLOUD_INSTANTIATION(Traits) \
	extern template class tree_point_cloud<Traits, point_xyz>; \
	extern template class tree_point_cloud<Traits, point_full>;


#define PCF_DEFINE_TREE_POINT_CLOUD_INSTANTIATION(Traits) \
	template class tree_point_cloud<Traits, point_xyz>; \
	template class tree_point_cloud<Traits, point_full>;



namespace pcf {

/**
Point cloud arranged into tree structure.
*/
template<typename Traits, typename Point, typename Allocator = default_allocator<Point>>
class tree_point_cloud : public point_cloud<Point, Allocator> {
	using super = point_cloud<Point, Allocator>;
	using segment = typename super::segment;
	using const_segment = typename super::const_segment;
	using node_attributes = typename Traits::node_attributes;

private:
	/**
	Node of the tree.
	Kept in memory. To minimize size, does not contain node's bounding box or other data that can be 
	computed while traversing tree. Inherits from node_attributes for empty base optimization.
	node_handle is used to access and traverse tree.
	*/
	struct node : node_attributes {
		segment seg; ///< Point cloud segment corresponding to this node.
		std::array<std::unique_ptr<node>, Traits::number_of_children> children; ///< Pointers to child nodes. May be null.
		
		node() = default;
		node(const node&) = delete;
		node(node&& nd) : seg(nd.seg), children(std::move(nd.children)) { }
		explicit node(const segment& seg) :
			seg(seg) { }
	};

	static std::size_t compute_leaf_capacity_(std::size_t requested, bool round_up_to_page_size);

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
	template<typename Other_cloud>
	tree_point_cloud(const Other_cloud&, std::size_t leaf_cap = 0, bool round_up_to_page_size = true, const Allocator& = Allocator());
	template<typename Other_cloud>
	tree_point_cloud(Other_cloud&&, std::size_t leaf_cap = 0, bool round_up_to_page_size = true);

	tree_point_cloud(const tree_point_cloud& pc) : tree_point_cloud(pc, 0) { }

	tree_point_cloud(super&&, std::size_t leaf_cap = 0, bool round_up_to_page_size = true);
	tree_point_cloud(tree_point_cloud&&);

	node_handle root();
	const_node_handle root() const;

	bool verify() const { return verify_(root()); }

	const Point& closest_point(const point_xyz& query, float accepting_distance = 0, float rejecting_distance = INFINITY) const;
};

}

#include "tree_point_cloud.tcc"

#endif
