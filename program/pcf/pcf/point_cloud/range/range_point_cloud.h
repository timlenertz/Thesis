#ifndef PCF_RANGE_POINT_CLOUD_H_
#define PCF_RANGE_POINT_CLOUD_H_

#include <iostream>
#include <vector>
#include <limits>
#include "../../geometry/camera.h"
#include "../point_cloud.h"
#include "../../range_image.h"

namespace pcf {

template<typename Point, typename Allocator = aligned_allocator<Point>>
class range_point_cloud : public point_cloud<Point, Allocator> {
	using super = point_cloud<Point, Allocator>;

public:
	using image_coordinates = std::array<std::ptrdiff_t, 2>;
	using angular_image_coordinates = std::array<angle, 2>;

private:
	camera camera_;
	std::array<float, 2> angular_resolution_;
	std::array<std::size_t, 2> image_size_;
	std::array<std::ptrdiff_t, 2> image_center_;

	std::ptrdiff_t offset_(image_coordinates) const;
	angular_image_coordinates to_angular(image_coordinates) const;
	image_coordinates to_image(angular_image_coordinates) const;
	image_coordinates to_image(const spherical_coordinates&) const;
	static std::size_t number_of_pixels_for_camera_(angle angular_res_x, angle angular_res_y, const camera&);

	template<typename Other_cloud> void project_(const Other_cloud&);

public:
	template<typename Other_cloud>
	range_point_cloud(const Other_cloud&, const camera&, angle angular_res_x, angle angular_res_y = 0, const Allocator& = Allocator());

	range_point_cloud(const range_image&, const camera&, const Allocator& = Allocator());

	template<typename Reader> explicit range_point_cloud(Reader&, const Allocator& = Allocator());

	camera& get_camera() { return camera_; }
	const camera& get_camera() const { return camera_; }
	
	std::size_t width() const { return image_size_[0]; }
	std::size_t height() const { return image_size_[1]; }
	
	Point& at(image_coordinates c) { return super::operator[](offset_(c)); }
	const Point& at(image_coordinates c) const { return super::operator[](offset_(c)); }
	
	bool in_bounds(image_coordinates) const;
			
	template<typename Other_point, typename Distance_func>
	const Point& find_closest_point(const Other_point& from, Distance_func dist, unsigned neightborhood_radius = 30) const;
		
	range_image to_range_image();
};

using range_point_cloud_xyz = range_point_cloud<point_xyz>;
using range_point_cloud_full = range_point_cloud<point_full>;

}

#include "range_point_cloud.tcc"

#endif