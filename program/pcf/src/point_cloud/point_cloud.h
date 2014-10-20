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
#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include "../asset.h"
#include "../point.h"
#include "../geometry/cuboid.h"

namespace pcf {

/**
Set of points laid out in memory.
Allocates to given capacity using provided allocator. Allocated size cannot change, but actual size is variable and can be smaller.
This base considers the point cloud as unordered point set, subclasses implement structure by imposing point order.
Depending on all_valid_ option, may or may not contain invalid points.
*/
template<typename Point, typename Allocator = std::allocator<Point>>
class point_cloud : public asset {
public:
	using point_type = Point;
	using iterator = Point*;
	using const_iterator = const Point*;

private:
	Allocator allocator_; ///< Allocator used to create buffer. Only one allocation and deallocation during object lifetime.

protected:
	Point* buffer_; ///< Start of points buffer. If null, has been invalidated from move constructor.
	Point* buffer_end_; ///< Current end of points buffer. May be set shorter than allocated buffer end.
	const std::size_t allocated_size_; ///< Allocated buffer size.
	
	const bool all_valid_; ///< If set, all points must be valid.
	
	void check_correct_alignment_() const;

	point_cloud(std::size_t allocate_size, bool all_valid, const Allocator& alloc = Allocator());

	void resize_(std::size_t new_size);
	void initialize_();

	static const Point& invalid_point_();

public:
	point_cloud() = delete;
	point_cloud(const point_cloud&, bool all_val = false);
	point_cloud(point_cloud&&, bool all_val = false);
	
	template<typename Other_point, typename Other_allocator>
	point_cloud(const point_cloud<Other_point, Other_allocator>& pc, bool all_val = false, const Allocator& alloc = Allocator());

	point_cloud& operator=(const point_cloud&);
	point_cloud& operator=(point_cloud&&);
	template<typename Other> point_cloud& operator=(const Other&);

	~point_cloud();

	template<typename Reader> static point_cloud create_from_reader(Reader&, bool all_valid = true);
	template<typename Reader> void read(Reader&);
	template<typename Writer> void write(Writer&) const;
	
	bool all_valid() const { return all_valid_; }
	std::size_t capacity() const { return allocated_size_; }
	std::size_t size() const { return buffer_end_ - buffer_; }
	std::size_t number_of_valid_points() const;
	bool contains_invalid_points() const;
	
	bool in_bounds(std::ptrdiff_t i) const { return i >= 0 && i < size(); }
	Point& operator[](std::ptrdiff_t i) { assert(in_bounds(i)); return buffer_[i]; }
	const Point& operator[](std::ptrdiff_t i) const { assert(in_bounds(i)); return buffer_[i]; }
	
	Point* data() { return buffer_; }
	const Point* data() const { return buffer_; }
	const Point* data_end() const { return buffer_end_; }
	
	iterator begin() { return buffer_; }
	const_iterator begin() const { return buffer_; }
	const_iterator cbegin() const { return buffer_; }
	iterator end() { return buffer_end_; }
	const_iterator end() const { return buffer_end_; }
	const_iterator cend() const { return buffer_end_; }
	
	template<typename Transformation>
	void apply_transformation(const Transformation& t);
	
	Eigen::Vector3f center_of_mass() const;
	cuboid bounding_cuboid(float ep = 0) const;
	
	template<typename Other_point, typename Distance_func>
	const Point& find_closest_point(const Other_point& from, Distance_func dist, const Point* start = nullptr, const Point* end = nullptr) const;
	
	void erase_invalid_points();
		
	template<typename Random_generator = std::default_random_engine>
	void downsample_random(float ratio, bool invalidate = false);
	
	template<typename Compare_func>
	void sort_points(Compare_func func, Point* start = nullptr, Point* end = nullptr);
};

}

#include "point_cloud.tcc"

#endif