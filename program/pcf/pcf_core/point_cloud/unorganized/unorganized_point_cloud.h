#ifndef PCF_UNORGANIZED_POINT_CLOUD_H_
#define PCF_UNORGANIZED_POINT_CLOUD_H_

#include "../filter_point_cloud.h"
#include <cmath>
#include <utility>

namespace pcf {

class point_cloud_importer;

/**
Unorganized point cloud.
Point cloud stored as an array of points in any order. Provides algorithms that operate on raw point set and would destroy organized point cloud. Models are imported as unorganized point clouds. Structured point clouds (grid, tree, etc) are created by copy or move construction from unorganized point cloud or any other organized point cloud. With move construction same memory is reused.
*/
template<typename Point, typename Allocator = default_allocator<Point>>
class unorganized_point_cloud : public filter_point_cloud<Point, Allocator> {
	using super = filter_point_cloud<Point, Allocator>;

public:
	explicit unorganized_point_cloud(std::size_t size, std::size_t capacity = 0, const Allocator& alloc = Allocator());

	/// Copy-construct from existing point cloud.
	/// If all_val and other is not all-valid, erases invalid points.
	template<typename Other_point, typename Other_allocator>
	unorganized_point_cloud(const point_cloud<Other_point, Other_allocator>& pc, std::size_t capacity = 0, const Allocator& alloc = Allocator()) :
		super(pc, capacity, alloc) { }
	
	/// Copy-construct from existing point cloud.
	unorganized_point_cloud(const unorganized_point_cloud& pc) :
		unorganized_point_cloud(pc, 0) {  }

	/// Move-construct from existing point cloud.
	/// Does not copy or allocate memory.
	unorganized_point_cloud(point_cloud<Point, Allocator>&& pc) :
		super(std::move(pc)) { }
	unorganized_point_cloud(unorganized_point_cloud&& pc) :
		super(std::move(pc)) { }
	
	/// Create point cloud from importer.
	explicit unorganized_point_cloud(point_cloud_importer&, const Allocator& = Allocator());
	
	template<typename Other_iterator>
	typename super::iterator insert(typename super::iterator pos, Other_iterator begin, Other_iterator end);

	/// Erase invalid points from the point cloud.
	void erase_invalid_points();	
	
	/// Apply transformation to all points in point cloud.
	void apply_transformation(const Eigen::Affine3f&);
	
	/// Applies transformation corresponding to space object's pose.
	/// Applies the transformation of the space object's pose relative to its parent, and then sets this pose to identity.
	void apply_pose();
		
	/// Randomize internal order of points.
	void shuffle();

	/// Displace points using random number distribution.
	/// Random numbers generated using the given distribution is added to the X, Y and Z coordinates of each point.
	template<typename Distribution>
	void randomly_displace_points(Distribution);

	template<typename Distribution>
	void randomly_displace_points_on_local_surface(Distribution);

	/// Add random noise points around the points.
	/// Number of points to add given by \p amount. \p amount must be lesser or equal to remaining capacity, and lesser or equal to number of points in cloud. I.e. at most one noise point is added per existing point.
	template<typename Distribution>
	void add_random_noise_around_points(std::size_t amount, const Distribution& displacement);
	
	/// Add random noise within given bounding box.
	/// Added points get uniformly distributed in cuboid.  \p amount must be lesser or equal to remaining capacity.
	void add_random_noise_in_box(std::size_t amount, const bounding_box& box);

	/// Find closest point to given query point.
	/// Trivial implementation; tests every point. Structured point clouds provide more efficient algorithms.
	template<typename Other_point>
	const Point& closest_point(const Other_point&, float accepting_distance = 0, float rejecting_distance = INFINITY) const;
	
	template<typename Condition_func, typename Callback_func>
	void nearest_neighbors(std::size_t k, Condition_func cond, Callback_func callback, bool parallel = false) const;
	
	template<typename Condition_func, typename Callback_func>
	void nearest_neighbors(std::size_t k, Condition_func cond, Callback_func callback, bool parallel = false);
};

extern template class unorganized_point_cloud<point_xyz>;
extern template class unorganized_point_cloud<point_full>;

using unorganized_point_cloud_xyz = unorganized_point_cloud<point_xyz>;
using unorganized_point_cloud_full = unorganized_point_cloud<point_full>;

}

#include "unorganized_point_cloud.tcc"

#endif