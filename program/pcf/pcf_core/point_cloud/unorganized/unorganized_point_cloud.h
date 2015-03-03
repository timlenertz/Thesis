#ifndef PCF_UNORGANIZED_POINT_CLOUD_H_
#define PCF_UNORGANIZED_POINT_CLOUD_H_

#include "../point_cloud.h"
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

private:
	bool filters_invalidate_points_ = false;

public:
	/// Copy-construct from existing point cloud.
	/// If all_val and other is not all-valid, erases invalid points.
	template<typename Other_point, typename Other_allocator>
	unorganized_point_cloud(const point_cloud<Other_point, Other_allocator>&, std::size_t capacity = 0, bool all_val = true, const Allocator& = Allocator());

	/// Move-construct from existing point cloud.
	/// Does not copy or allocate memory.
	unorganized_point_cloud(super&&, bool all_val = true);

	/// Create point cloud from importer.
	explicit unorganized_point_cloud(point_cloud_importer&, const Allocator& = Allocator());

	bool filters_invalidate_points() const;
	void filters_invalidate_points(bool);

	/// Erase invalid points from the point cloud.
	void erase_invalid_points();	
	
	/// Erase all points that are not accepted by filter.
	template<typename Filter>
	void filter(Filter, bool parallel = true);
	
	/// Apply transformation to all points in point cloud.
	void apply_transformation(const Eigen::Affine3f&);
	
	/// Applies transformation corresponding to space object's pose.
	/// Applies the transformation of the space object's pose relative to its parent, and then sets this pose to identity.
	void apply_pose();
	
	/// Randomize internal order of points.
	void shuffle();

	/// Apply random downsampling, leaving only the given ratio of points.
	/// If invalidate is set, invalidates point to erase, but does not move points around, thus the remaining points maintain the same indices. Only possible if cloud is not all valid. If not set, erases the points and moves remaining points together.
	void downsample_random(float ratio);
	
	/// Apply random downsampling, where ratio depends on position.
	/// \p Field is a function taking an Eigen::Vector3f as argument, and returning a float value in [0, 1], indicating the downsampling ratio at that location. (I.e. the probability that a point at that location gets erased.)
	template<typename Field>
	void downsample_random_field(const Field&);
	
	/// Apply grid downsampling, using given grid cell side length.
	/// Puts regular axis-aligned cubic grid over point cloud, and keeps exactly one point for each grid cell that contained at least one point. If keep_first, keeps the first point found to be inside a cell. Else, keeps the point closest to the cell's center. Does not displace points.
	void downsample_grid(float cell_sz, bool keep_first = false);
	
	/// Project using the given image camera, and keep only points on image.
	template<typename Image_camera>
	void downsample_projection(const Image_camera&);

	/// Displace points using random number distribution.
	/// Random numbers generated using the given distribution is added to the X, Y and Z coordinates of each point.
	template<typename Distribution>
	void randomly_displace_points(const Distribution&);

	/// Add random noise points around the points.
	/// Number of points to add given by \p amount. \p amount must be lesser or equal to remaining capacity, and lesser or equal to number of points in cloud. I.e. at most one noise point is added per existing point.
	template<typename Distribution>
	void add_random_noise_around_points(std::size_t amount, const Distribution& displacement);
	
	/// Add random noise within given bounding box.
	/// Added points get uniformly distributed in cuboid.  \p amount must be lesser or equal to remaining capacity.
	void add_random_noise_in_box(std::size_t amount, const bounding_box& box);
	
	/// Keep only points that are visible to given camera.
	/// Camera must be an image_camera. Applies z-buffering and keeps only closest point for each image pixel.
	template<typename Camera>
	void erase_invisible_points(const Camera&);

	/// Find closest point to given query point.
	/// Trivial implementation; tests against point. Structured point clouds provide more efficient algorithms.
	template<typename Other_point>
	const Point& closest_point(const Other_point&, float accepting_distance = 0, float rejecting_distance = INFINITY) const;
};

extern template class unorganized_point_cloud<point_xyz>;
extern template class unorganized_point_cloud<point_full>;

using unorganized_point_cloud_xyz = unorganized_point_cloud<point_xyz>;
using unorganized_point_cloud_full = unorganized_point_cloud<point_full>;

}

#include "unorganized_point_cloud.tcc"

#endif