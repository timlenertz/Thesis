#ifndef PCFVW_POV_POINT_CLOUD_H_
#define PCFVW_POV_POINT_CLOUD_H_

#include "../../pcf/point_cloud/tree/tree_point_cloud.h"
#include "pov_octree_traits.h"

namespace pcf {

class camera;

template<typename Point, typename Allocator = aligned_allocator<Point>>
class pov_point_cloud : public tree_point_cloud<pov_octree_traits, Point, Allocator> {
	using super = tree_point_cloud<pov_octree_traits, Point, Allocator>;
	using typename super::node_handle;
	using typename super::const_node_handle;
	
private:
	static const std::size_t leaf_capacity_ = 1000;
	float extra_split_side_length_ = 2.0;

	void prepare_tree_();	
	void extract_points_(Point* buffer, std::size_t n, const const_node_handle& nd) const;
	
public:
	template<typename Other_cloud> pov_point_cloud(Other_cloud&&, const Allocator&);
	template<typename Other_cloud> explicit pov_point_cloud(Other_cloud&&);

	std::size_t extract(Point* buffer, std::size_t capacity, const camera&) const;
};

using pov_point_cloud_xyz = pov_point_cloud<point_xyz>;
using pov_point_cloud_full = pov_point_cloud<point_full>;

}

#include "pov_point_cloud.tcc"

#endif
