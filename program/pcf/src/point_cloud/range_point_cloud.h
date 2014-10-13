#ifndef PCF_RANGE_POINT_CLOUD_H_
#define PCF_RANGE_POINT_CLOUD_H_

#include <iostream>
#include <vector>
#include <limits>
#include "point_cloud.h"
#include "../range_image.h"

namespace pcf {

template<typename Point, typename Allocator = std::allocator<Point>>
class range_point_cloud : public point_cloud<Point, Allocator> {
public:
	struct image_coordinates { std::ptrdiff_t x, y; };

private:
	using super = point_cloud<Point, Allocator>;
	
	const std::size_t width_, height_;
	Eigen::Projective3f projection_matrix_;

	std::ptrdiff_t offset_(std::ptrdiff_t x, std::ptrdiff_t y) const {
		return y*width_ + x;
	}

public:
	range_point_cloud(std::size_t w, std::size_t h, const Allocator& alloc = Allocator()) :
	super(w * h, alloc), width_(w), height_(h) { }

	std::size_t width() const { return width_; }
	std::size_t height() const { return height_; }

	void erase();
	
	Point& at(const image_coordinates& ic) { return super::buffer_[ offset_(ic.x, ic.y) ]; }
	const Point& at(const image_coordinates& ic) const { return super::buffer_[ offset_(ic.x, ic.y) ]; }
	
	bool in_bounds(const image_coordinates& ic) const {
		return ic.x >= 0 && ic.x < width_ && ic.y >= 0 && ic.y < height_;
	}
	
	template<typename Other_point> image_coordinates project(const Other_point& p);
	template<typename Other_point> image_coordinates project(const Other_point& p, float& depth);
	
	float depth(const Point& p) const;
	
	template<typename Transformation>
	void apply_transformation(const Transformation&);
	
	
	template<typename Other_cloud>
	void project_point_cloud(const Other_cloud& pc, const Eigen::Projective3f& projm);
	
	range_image to_range_image();
};

}

#include "range_point_cloud.tcc"

#endif