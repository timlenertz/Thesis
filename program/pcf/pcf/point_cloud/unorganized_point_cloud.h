#ifndef PCF_UNORGANIZED_POINT_CLOUD_H_
#define PCF_UNORGANIZED_POINT_CLOUD_H_

#include "point_cloud.h"
#include <cmath>

namespace pcf {

class point_cloud_importer;

/**
Unorganized point cloud.
Point cloud stored as an array of points in any order. Provides algorithms that operate on raw point set and would destroy organized point cloud. Models are imported as unorganized point clouds. Structured point clouds (grid, tree, etc) are created by copy or move construction from unorganized point cloud or any other organized point cloud. With move construction same memory is reused.
*/
template<typename Point, typename Allocator = aligned_allocator<Point>>
class unorganized_point_cloud : public point_cloud<Point, Allocator> {
	using super = point_cloud<Point, Allocator>;

public:
	unorganized_point_cloud(const unorganized_point_cloud& pc, bool all_val = true) :
		unorganized_point_cloud(static_cast<super&>(pc), all_val) { }
	unorganized_point_cloud(unorganized_point_cloud&& pc, bool all_val = true) :
		unorganized_point_cloud(static_cast<super&>(pc), all_val) { }


	unorganized_point_cloud(const super&, bool all_val = true);
	unorganized_point_cloud(super&&, bool all_val = true);

	explicit unorganized_point_cloud(point_cloud_importer&, const Allocator& = Allocator());

	template<typename Other_point, typename Other_allocator>
	unorganized_point_cloud(const point_cloud<Other_point, Other_allocator>&, bool all_val = false, const Allocator& = Allocator());
		
	void erase_invalid_points();	
	
	void apply_transformation(const Eigen::Affine3f&);
	void apply_pose();

	void downsample_random(float ratio, bool invalidate = false);
	
	void downsample_grid(float cell_sz, bool move = true, bool invalidate = false);

	template<typename Other_point>
	const Point& closest_point(const Other_point&, float accepting_distance = 0, float rejecting_distance = INFINITY) const;
};

using unorganized_point_cloud_xyz = unorganized_point_cloud<point_xyz>;
using unorganized_point_cloud_full = unorganized_point_cloud<point_full>;

}

#include "unorganized_point_cloud.tcc"

#endif