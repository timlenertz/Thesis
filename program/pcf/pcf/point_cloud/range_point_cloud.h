#ifndef PCF_RANGE_POINT_CLOUD_H_
#define PCF_RANGE_POINT_CLOUD_H_

#include <iostream>
#include <vector>
#include <limits>
#include "../geometry/camera.h"
#include "point_cloud.h"
#include "../range_image.h"

namespace pcf {

template<typename Point, typename Allocator = aligned_allocator<Point>>
class range_point_cloud : public point_cloud<Point, Allocator> {
	using super = point_cloud<Point, Allocator>;

public:
	using image_coordinates = camera::image_coordinates;

private:
	camera camera_;

	std::ptrdiff_t offset_(image_coordinates);

public:
	template<typename Other_cloud>
	range_point_cloud(const Other_cloud&, const camera&, const Allocator& alloc = Allocator());

	camera& get_camera() { return camera_; }
	const camera& get_camera() const { return camera_; }
	
	Point& at(image_coordinates ic) { return super::operator[](offset_(ic.x, ic.y)); }
	const Point& at(image_coordinates ic) { return super::operator[](offset_(ic.x, ic.y)); }
	
	bool in_bounds(image_coordinates ic) const;
			
	template<typename Other_point, typename Distance_func>
	const Point& find_closest_point(const Other_point& from, Distance_func dist, unsigned neightborhood_radius = 30) const;
		
	range_image to_range_image();
};

using range_point_cloud_xyz = range_point_cloud<point_xyz>;
using range_point_cloud_full = range_point_cloud<point_full>;

}

#include "range_point_cloud.tcc"

#endif