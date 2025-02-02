#ifndef PCFVW_POV_POINT_CLOUD_H_
#define PCFVW_POV_POINT_CLOUD_H_

#include "../../pcf_core/point_cloud/tree/tree_point_cloud.h"
#include "pov_octree_traits.h"

namespace pcf {

struct frustum;

/**
Point cloud structured for fast extraction of points visible from given point of view.
Uses octree point cloud. Performs view frustum culling and online random downsampling.
*/
template<typename Point, typename Allocator = aligned_allocator<Point>>
class pov_point_cloud : public tree_point_cloud<pov_octree_traits, Point, Allocator> {
	using super = tree_point_cloud<pov_octree_traits, Point, Allocator>;
	using typename super::node_handle;
	using typename super::const_node_handle;
	
private:
	float extra_split_side_length_ = 2.0;

	void prepare_tree_();	
	void extract_points_(Point* buffer, std::size_t n, const const_node_handle& nd) const;
	
public:
	template<typename Other_cloud> pov_point_cloud(Other_cloud&&, const Allocator& = Allocator());

	/// Extract points for given viewing frustum.
	/// Frustum must be given in point cloud's coordinate system.
	std::size_t extract(Point* buffer, std::size_t capacity, const frustum&) const;
};

extern template class pov_point_cloud<point_xyz>;
extern template class pov_point_cloud<point_full>;

using pov_point_cloud_xyz = pov_point_cloud<point_xyz>;
using pov_point_cloud_full = pov_point_cloud<point_full>;

}

#include "pov_point_cloud.tcc"

#endif
