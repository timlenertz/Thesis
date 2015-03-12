#ifndef PCF_FILTER_POINT_CLOUD_H_
#define PCF_FILTER_POINT_CLOUD_H_

#include "point_cloud.h"
#include <cmath>
#include <utility>
#include <vector>

namespace pcf {

/**
Base class for point cloud with filters that may invalidate points.
Implements filters that invalidate points, but don't otherwise change them, or add/remove points. Inherited by unorganized point cloud, and range point cloud. But not by structured point clouds, because they are all-valid or because this would damage the structure.
*/
template<typename Point, typename Allocator = default_allocator<Point>>
class filter_point_cloud : public point_cloud<Point, Allocator> {
	using super = point_cloud<Point, Allocator>;

public:
	filter_point_cloud(std::size_t allocate_size, const Allocator& alloc) :
		super(allocate_size, false, alloc) { }

	filter_point_cloud(filter_point_cloud&& pc) : super(std::move(pc), false) { }
	filter_point_cloud(super&& pc) : super(std::move(pc), false) { }

	template<typename Other_point, typename Other_allocator>
	filter_point_cloud(const point_cloud<Other_point, Other_allocator>& pc, std::size_t capacity, const Allocator& alloc) :
		super(pc, false, capacity, alloc) { }


	/// Invalidate all points that are not accepted by filter.
	/// Filter is function object that gets called for each point with the point as argument. If it returns false, that point is invalidated.
	template<typename Filter>
	void filter(Filter, bool parallel = true);
	
	/// Invalidate all points according to mask.
	/// Mask is forward iterable, where each item evaluated to boolean. For example vector<bool>. If an item is false, then the point at that index is invalidated. When inverse is set, the point is invalidated if the item is true instead. Never revalidates invalid points.
	template<typename Mask>
	void filter_mask(const Mask&, bool inverse = false);
	
	/// Returns mask where valid is true for each valid point;
	std::vector<bool> valid_points_mask() const;
	
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

	/// Keep only points that are visible to given camera.
	/// Camera must be an image_camera. Applies z-buffering and keeps only closest point for each image pixel.
	template<typename Camera>
	void erase_invisible_points(const Camera&);
};

extern template class filter_point_cloud<point_xyz>;
extern template class filter_point_cloud<point_full>;

using filter_point_cloud_xyz = filter_point_cloud<point_xyz>;
using filter_point_cloud_full = filter_point_cloud<point_full>;

}

#include "filter_point_cloud.tcc"

#endif