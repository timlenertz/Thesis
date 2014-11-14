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
#include "../point.h"
#include "../util/aligned_allocator.h"
#include "segment.h"
#include "segment_union.h"

namespace pcf {

/**
Set of points laid out in memory.
Allocates to given capacity using provided allocator. Allocated size cannot change, but actual size is variable and can be made smaller. Considers the point cloud as unordered point set, subclasses implement structure by imposing point order.
Depending on all_valid_ option, may or may not contain invalid points.
*/
template<typename Point, typename Allocator = aligned_allocator<Point>>
class point_cloud : public point_cloud_segment<Point> {
	using super = point_cloud_segment<Point>;
	
protected:
	Allocator allocator_; ///< Allocator used to create buffer.
	const std::size_t allocated_size_; ///< Allocated buffer size.
	const bool all_valid_; ///< If set, all points must be valid.
	
	void check_correct_alignment_() const;

	point_cloud(std::size_t allocate_size, bool all_valid, const Allocator& alloc = Allocator());

	void resize_(std::size_t new_size);
	void initialize_();

	static const Point& invalid_point_();

public:
	using segment = point_cloud_segment<Point>;
	using const_segment = point_cloud_segment<const Point>;
	using segment_union = point_cloud_segment_union<Point>;
	using const_segment_union = point_cloud_segment_union<const Point>;

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
	std::size_t size() const { return super::size(); }
	std::size_t number_of_valid_points() const;
	bool contains_invalid_points() const;

	void erase_invalid_points();
			
	template<typename Transformation>
	void apply_transformation(const Transformation& t);	
		
	template<typename Random_generator = std::default_random_engine>
	void downsample_random(float ratio, bool invalidate = false);

	template<typename Other_point>
	const Point& find_closest_point(const Other_point&) const;
};

using point_cloud_xyz = point_cloud<point_xyz>;
using point_cloud_full = point_cloud<point_full>;

}

#include "point_cloud.tcc"

#endif