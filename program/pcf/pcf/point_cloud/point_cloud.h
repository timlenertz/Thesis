#ifndef PCF_POINT_CLOUD_H_
#define PCF_POINT_CLOUD_H_

#include <cstdint>
#include <stdexcept>
#include <cassert>
#include <utility>
#include <memory>
#include <limits>
#include <cstring>
#include <random>
#include <cmath>
#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include "../point.h"
#include "../util/aligned_allocator.h"
#include "segment.h"
#include "segment_union.h"

namespace pcf {

/**
Set of points laid out in memory.
Allocates to given capacity using provided allocator. Allocated size cannot change, but actual size is variable and can be made smaller. Considers the point cloud as unordered point set, subclasses implement structure by imposing point order. Memory is never reallocated, so
point addresses are constant. Depending on all_valid_ option, may or may not contain invalid points.
*/
template<typename Point, typename Allocator = aligned_allocator<Point>>
class point_cloud {	
protected:
	Allocator allocator_; ///< Allocator used to create buffer.
	const std::size_t allocated_size_; ///< Allocated buffer size.
	const bool all_valid_; ///< If set, all points must be valid.
	Point* begin_; ///< Point buffer. Allocated and owned by point cloud.
	Point* end_; ///< End of point buffer. Can be changed, must be in begin_ + [0; allocated_size_]
	
	void check_correct_alignment_() const;

	/**
	Create empty point cloud.
	Allocates memory and creates uninitialized point cloud of size 0. Needs to be resized using resize_, and
	filled with content by subclass constructor.
	*/
	point_cloud(std::size_t allocate_size, bool all_valid, const Allocator& alloc = Allocator());

	/**
	Set size of point cloud.
	Needs to range from 0 to allocated size. Does not reallocate memory.
	*/
	void resize_(std::size_t new_size);
	
	/**
	Initialize all points to invalid point.
	*/
	void initialize_();

	static const Point& invalid_point_();

public:
	using point_type = Point;

	using segment = point_cloud_segment<Point>;
	using const_segment = point_cloud_segment<const Point>;
	using segment_union = point_cloud_segment_union<Point>;
	using const_segment_union = point_cloud_segment_union<const Point>;

	using iterator = typename segment::iterator;
	using const_iterator = typename const_segment::iterator;
	
	point_cloud() = delete;
	point_cloud(const point_cloud&, bool all_val = true);
	point_cloud(point_cloud&&, bool all_val = true);
	
	template<typename Other_point, typename Other_allocator>
	point_cloud(const point_cloud<Other_point, Other_allocator>& pc, bool all_val = false, const Allocator& alloc = Allocator());

	~point_cloud();

	point_cloud& operator=(const point_cloud&);
	point_cloud& operator=(point_cloud&&);
	template<typename Other> point_cloud& operator=(const Other&);

	template<typename Reader> static point_cloud create_from_reader(Reader&, bool all_valid = true);
	template<typename Reader> void read(Reader&);
	template<typename Writer> void write(Writer&) const;
	
	bool all_valid() const { return all_valid_; }
	std::size_t capacity() const { return allocated_size_; }
	std::size_t size() const { return end_ - begin_; }
	bool empty() const { return end_ == begin_; }
	
	std::size_t number_of_valid_points() const;
	bool contains_invalid_points() const;

	segment full_segment() { return segment(begin_, end_); }
	const_segment full_segment() const { return segment(begin_, end_); }	

	Point* data() { return begin_; }
	const Point* data() const { return begin_; }
	const Point* cdata() const { return begin_; }
	
	iterator begin() { return full_segment().begin(); }
	const_iterator begin() const { return full_segment().begin(); }
	const_iterator cbegin() const { return full_segment().begin(); }

	iterator end() { return full_segment().end(); }
	const_iterator end() const { return full_segment().end(); }
	const_iterator cend() const { return full_segment().end(); }

	bounding_box box(float ep = 0.0) const { return full_segment().box(ep); }
	Eigen::Vector3f center_of_mass() const { return full_segment().center_of_mass(); }

	void erase_invalid_points();
			
	void apply_transformation(const Eigen::Affine3f&);
	
	template<typename Callback> void with_transformation(const Eigen::Affine3f&, const Callback&);
	template<typename Callback> void with_transformation(const Eigen::Affine3f&, const Callback&) const;
	
		
	template<typename Random_generator = std::default_random_engine>
	void downsample_random(float ratio, bool invalidate = false);
	
	void downsample_grid(float cell_sz, bool move = true, bool invalidate = false);

	template<typename Other_point>
	const Point& closest_point(const Other_point&, float accepting_distance = 0, float rejecting_distance = INFINITY) const;
};

using point_cloud_xyz = point_cloud<point_xyz>;
using point_cloud_full = point_cloud<point_full>;

}

#include "point_cloud.tcc"

#endif