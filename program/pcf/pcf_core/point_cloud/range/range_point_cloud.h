#ifndef PCF_RANGE_POINT_CLOUD_H_
#define PCF_RANGE_POINT_CLOUD_H_

#include <vector>
#include <limits>
#include <memory>
#include <cmath>
#include "../../rgb_color.h"
#include "../point_cloud.h"
#include "../../util/multi_dimensional_buffer.h"

namespace pcf {

class range_point_cloud_importer;
class range_image;
class color_image;

/**
Point cloud corresponding to range image.
*/
template<typename Point, typename Allocator = default_allocator<Point>>
class range_point_cloud : public point_cloud<Point, Allocator> {
	using super = point_cloud<Point, Allocator>;

protected:
	range_point_cloud(std::size_t w, std::size_t h, const Allocator&);
	multi_dimensional_buffer<Point, 2> image_;

public:
	explicit range_point_cloud(range_point_cloud_importer&, const Allocator& = Allocator());
	
	std::size_t width() const;
	std::size_t height() const;
		
	range_image to_range_image() const;
	color_image to_color_image(rgb_color bg = rgb_color::black) const;
	
	void crop(std::ptrdiff_t x_min, std::ptrdiff_t x_max, std::ptrdiff_t y_min, std::ptrdiff_t y_max);
	void crop_depth(float depth_min = 0, float depth_max = INFINITY);
};

using range_point_cloud_xyz = range_point_cloud<point_xyz>;
using range_point_cloud_full = range_point_cloud<point_full>;

extern template class range_point_cloud<point_xyz>;
extern template class range_point_cloud<point_full>;

}

#include "range_point_cloud.tcc"

#endif