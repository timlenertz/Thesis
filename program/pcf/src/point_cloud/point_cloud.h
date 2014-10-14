#ifndef PCF_POINT_CLOUD_H_
#define PCF_POINT_CLOUD_H_

#include <cstdint>
#include <stdexcept>
#include <utility>
#include <memory>
#include <limits>
#include <cstring>
#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include "../asset.h"
#include "../point.h"

namespace pcf {

template<typename Point, typename Allocator = std::allocator<Point>>
class point_cloud : public asset {
public:
	using point_type = Point;
	using iterator = Point*;
	using const_iterator = const Point*;

private:
	Allocator allocator_;

protected:
	Point* const buffer_;
	Point* buffer_end_;
	const std::size_t allocated_size_;
	
	void check_correct_alignment_() const;

public:
	explicit point_cloud(std::size_t size, const Allocator& alloc = Allocator());
	
	point_cloud(const point_cloud&, const Allocator& alloc = Allocator());
	template<typename Other_alloc> point_cloud(const point_cloud<Point, Other_alloc>& pc, const Allocator& alloc = Allocator());
	template<typename Other> point_cloud(const Other& pc, const Allocator& alloc = Allocator());

	~point_cloud();

	template<typename Reader> static point_cloud create_from_reader(Reader&);
	template<typename Reader> void read(Reader&);
	template<typename Writer> void write(Writer&) const;
	
	std::size_t size() const { return buffer_end_ - buffer_; }
	std::size_t capacity() const { return allocated_size_; }

	void resize(std::size_t new_size);
	
	Point& operator[](std::ptrdiff_t d) { return buffer_[d]; }
	const Point& operator[](std::ptrdiff_t d) const { return buffer_[d]; }
	
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
	
	template<typename Other_point, typename Distance_func>
	Point& find_closest_point(const Other_point& from, Distance_func dist) const;
	
	void erase_invalid_points();
};

}

#include "point_cloud.tcc"

#endif