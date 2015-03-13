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
#include "../space_object.h"
#include "../util/default_allocator.h"
#include "segment.h"
#include "segment_union.h"
#include "../io/point_cloud_exporter.h"
#include "transform_iterator.h"

namespace pcf {

/**
Base class for point clouds.
Allocates to given capacity using provided allocator. Allocated size cannot change, but actual size is variable and can be made smaller. Memory is never reallocated, so point addresses are constant. Gives const-correct access to the points. Depending on all_valid_ option, may or may not contain invalid points. Cannot be instanciated, instead subclasses are used.
*/
template<typename Point, typename Allocator = default_allocator<Point>>
class point_cloud : public space_object {
public:
	using allocator_type = Allocator;
		
protected:
	Allocator allocator_; ///< Allocator used to create buffer.
	std::size_t allocated_size_; ///< Allocated buffer size.
	const bool all_valid_; ///< If set, all points must be valid.
	Point* begin_; ///< Point buffer. Allocated and owned by point cloud.
	Point* end_; ///< End of point buffer. Can be changed, must be in begin_ + [0; allocated_size_]
	
	/// Verify that memory is properly aligned.
	void check_correct_alignment_() const;

	/// Create empty point cloud.
	/// Allocates memory and creates uninitialized point cloud of size 0. Needs to be resized using resize_, and
	/// filled with content by subclass constructor.
	point_cloud(std::size_t allocate_size, bool all_val, const Allocator&);

	/// Create point cloud by move construction from another.
	/// Other's buffer is taken, and the other is invalidated so it won't deallocate it. No memory is allocated or copied.
	/// If all_val and other is not all-valid, erases invalid points. Takes allocator from other. Other must have same point and allocator types for move contruction.
	point_cloud(point_cloud&&, bool all_val);
	
	
	/// Create point cloud by copy construction from another.
	/// Allocates using provided allocator to size of other cloud, and copies points.
	/// If all_val and other is not all-valid, erases invalid points. If capacity == 0, takes size of given point cloud.
	template<typename Other_point, typename Other_allocator>
	point_cloud(const point_cloud<Other_point, Other_allocator>&, bool all_val, std::size_t capacity, const Allocator&);
		
	
	point_cloud() = delete; ///< Disallow default construction.
	point_cloud(const point_cloud&) = delete; ///< Disallow default copy-construction.
	point_cloud(point_cloud&&) = delete; ///< Disallow default move-construction.
	point_cloud& operator=(const point_cloud&) = delete; ///< Disallow assignment.
	

	/// Set size of point cloud.
	/// Needs to range from 0 to allocated size. Does not reallocate memory.
	void resize_(std::size_t new_size);
	
	/// Initialize all points to invalid point.
	void initialize_();

	static const Point& invalid_point_();
	
public:
	using point_type = Point;

	using segment = point_cloud_segment<Point>;
	using const_segment = point_cloud_segment<const Point>;
	using segment_union = point_cloud_segment_union<Point>;
	using const_segment_union = point_cloud_segment_union<const Point>;

	using iterator = Point*;
	using const_iterator = const Point*;

	using transform_iterator = point_transform_iterator<Point>;
	
	~point_cloud();
	
	const Allocator& get_allocator() const { return allocator_; }
	
	void export_with(point_cloud_exporter&) const;
	
	bool all_valid() const { return all_valid_; }
	std::size_t capacity() const { return allocated_size_; }
	std::size_t remaining_capacity() const { return capacity() - size(); }
	std::size_t size() const { return end_ - begin_; }
	bool empty() const { return end_ == begin_; }
	
	std::size_t number_of_valid_points() const;
	std::size_t number_of_invalid_points() const;
	bool contains_invalid_points() const;

	segment full_segment() { return segment(begin_, end_); }
	const_segment full_segment() const { return const_segment(begin_, end_); }	

	Point* data() { return begin_; }
	const Point* data() const { return begin_; }
	const Point* cdata() const { return begin_; }
	
	iterator begin() { return begin_; }
	const_iterator begin() const { return begin_; }
	const_iterator cbegin() const { return begin_; }

	iterator end() { return end_; }
	const_iterator end() const { return end_; }
	const_iterator cend() const { return end_; }
	
	transform_iterator begin_transform(const Eigen::Affine3f&) const;
	transform_iterator end_transform() const;
	
	transform_iterator begin_relative_to(const space_object&, const Eigen::Affine3f& transform = Eigen::Affine3f::Identity()) const;
	transform_iterator end_relative_to() const;
	
	const Point& random_point() const;
	Point& random_point();

	bounding_box box(float ep) const { return full_segment().box(ep); }
	bounding_box box() const override { return box(0); }
	
	Eigen::Vector3f center_of_mass() const { return full_segment().center_of_mass(); }
};

extern template class point_cloud<point_xyz>;
extern template class point_cloud<point_full>;

using point_cloud_xyz = point_cloud<point_xyz>;
using point_cloud_full = point_cloud<point_full>;

}

#include "point_cloud.tcc"

#endif