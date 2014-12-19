#ifndef PCF_RANGE_POINT_CLOUD_H_
#define PCF_RANGE_POINT_CLOUD_H_

#include <vector>
#include <limits>
#include <memory>
#include "../../image/range_image.h"
#include "../../image/range_image.h"
#include "../../rgb_color.h"
#include "../point_cloud.h"
#include "../../camera/range_image_camera.h"
#include "../../util/multi_dimensional_buffer.h"

namespace pcf {

class range_point_cloud_importer;

/**

*/
template<typename Point, typename Camera = range_image_camera, typename Allocator = default_allocator<Point>>
class range_point_cloud : public point_cloud<Point, Allocator> {
	using super = point_cloud<Point, Allocator>;

private:
	multi_dimensional_buffer<Point, 2> image_;
	Camera camera_;

public:
	template<typename Other_cloud>
	range_point_cloud(Other_cloud&& pc, const Camera&, const Allocator& alloc = Allocator());
	
	explicit range_point_cloud(range_point_cloud_importer&, const Allocator& = Allocator());
	
	std::size_t width() const;
	std::size_t height() const;
	
	const Camera& get_camera() const;
		
	range_image to_range_image() const;
	color_image to_color_image(rgb_color bg = rgb_color::black) const;
};

using range_point_cloud_xyz = range_point_cloud<point_xyz>;
using range_point_cloud_full = range_point_cloud<point_full>;

}

#include "range_point_cloud.tcc"

#endif