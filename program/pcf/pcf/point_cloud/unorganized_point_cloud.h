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
template<typename Point, typename Allocator = default_allocator<Point>>
class unorganized_point_cloud : public point_cloud<Point, Allocator> {
	using super = point_cloud<Point, Allocator>;

public:
	/// Copy-construct from existing point cloud.
	/// If all_val and other is not all-valid, erases invalid points.
	template<typename Other_point, typename Other_allocator>
	unorganized_point_cloud(const point_cloud<Other_point, Other_allocator>&, bool all_val = false, const Allocator& = Allocator());

	/// Move-construct from existing point cloud.
	/// Does not copy or allocate memory.
	unorganized_point_cloud(super&&, bool all_val = false);

	/// Create point cloud from importer.
	explicit unorganized_point_cloud(point_cloud_importer&, const Allocator& = Allocator());

	/// Erase invalid points from the point cloud.
	/// Does nothing if point cloud is all valid.
	void erase_invalid_points();	
	
	/// Erase all points that are not accepted by filter.
	template<typename Filter>
	void filter(Filter, bool invalidate = false, bool parallel = true);
	
	/// Apply transformation to all points in point cloud.
	void apply_transformation(const Eigen::Affine3f&);
	
	/// Applies transformation corresponding to space object's pose.
	/// Applies the transformation of the space object's pose relative to its parent, and then sets this pose to identity.
	void apply_pose();
	
	
	void shuffle();

	/// Apply random downsampling, leaving only the given ratio of points.
	/// If invalidate is set, invalidates point to erase, but does not move points around, thus the remaining points maintain the same indices. Only possible if cloud is not all valid. If not set, erases the points and moves remaining points together.
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