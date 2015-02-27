#ifndef PCF_RANGE_POINT_CLOUD_H_
#define PCF_RANGE_POINT_CLOUD_H_

#include <vector>
#include <limits>
#include <memory>
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

private:
	multi_dimensional_buffer<Point, 2> image_;

public:
	template<typename Other_cloud, typename Camera>
	range_point_cloud(Other_cloud&& pc, const Camera&, const Allocator& alloc = Allocator());
	
	explicit range_point_cloud(range_point_cloud_importer&, const Allocator& = Allocator());
	
	std::size_t width() const;
	std::size_t height() const;
		
	range_image to_range_image() const;
	color_image to_color_image(rgb_color bg = rgb_color::black) const;
};

using range_point_cloud_xyz = range_point_cloud<point_xyz>;
using range_point_cloud_full = range_point_cloud<point_full>;

extern template class range_point_cloud<point_xyz>;
extern template class range_point_cloud<point_full>;

}

#include "range_point_cloud.tcc"

#endif