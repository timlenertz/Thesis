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
		point_cloud_segment<Point> seg;
		std::array<std::unique_ptr<node>, Traits::number_of_children> children;
		
		node() = default;
		node(const segment& seg) : seg(seg) { }
	};
	
	const std::size_t leaf_capacity_;
	node root_node_;
	cuboid root_cuboid_;

	void build_tree_();
	bool verify_(const node&, const cuboid&, std::ptrdiff_t depth) const;

public:
	template<typename Other_cloud> tree_point_cloud(Other_cloud&& pc, std::size_t leaf_cap, const Allocator&);
	template<typename Other_cloud> tree_point_cloud(Other_cloud&& pc, std::size_t leaf_cap);

	bool verify() const;
};

}

#include "tree_point_cloud.tcc"

#endif
