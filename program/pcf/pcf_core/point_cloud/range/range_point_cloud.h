#ifndef PCF_RANGE_POINT_CLOUD_H_
#define PCF_RANGE_POINT_CLOUD_H_

#include <vector>
#include <limits>
#include <memory>
#include <cmath>
#include "../../rgb_color.h"
#include "../filter_point_cloud.h"
#include "../../util/multi_dimensional_buffer.h"
#include "../../util/coordinates.h"
#include "../../camera/range_image_camera.h"
#include "../../camera/projection_image_camera.h"
#include "../../image/image.h"
#include "../../image/masked_image.h"


namespace pcf {

class range_point_cloud_importer;
class range_image;
class rgb_color_image;

/**
Point cloud corresponding to range image.
Points are arranged in X-Y grid in memory, representing the elements (pixels) of the range image. Invalid point if no point exists in that direction. Point coordinates are such that camera is at origin, i.e. norm of a point is its distance to camera.
*/
template<typename Point, typename Allocator = default_allocator<Point>>
class range_point_cloud : public filter_point_cloud<Point, Allocator> {
	using super = filter_point_cloud<Point, Allocator>;

private:
	template<typename That, typename Condition_func, typename Callback_func>
	static void nearest_neighbors_(That that, std::size_t k, Condition_func cond, Callback_func callback, bool parallel);

protected:
	multi_dimensional_buffer<Point, 2> image_;
	const bool row_major_order_;

	static multi_dimensional_buffer<Point, 2> create_image_(bool row_major, std::size_t w, std::size_t h, Point* buf);

public:
	using image_coordinates = typename multi_dimensional_buffer<Point, 2>::indices_type;

	range_point_cloud(std::size_t w, std::size_t h, bool row_major = false, const Allocator& = Allocator());

	explicit range_point_cloud(range_point_cloud_importer&, const Allocator& = Allocator());
	
	template<typename Other_point, typename Other_allocate>
	range_point_cloud(const range_point_cloud<Other_point, Other_allocate>&, const Allocator& = Allocator());
	
	range_point_cloud(range_point_cloud&&);
	
	bool row_major_order() const { return row_major_order_; }
	std::size_t width() const;
	std::size_t height() const;
	
	const Point& at(std::ptrdiff_t x, std::ptrdiff_t y) const;
	Point& at(std::ptrdiff_t x, std::ptrdiff_t y);
		
	range_image to_range_image() const;
	masked_intensity_image weights_to_intensity_image() const;
	rgb_color_image to_rgb_color_image(rgb_color bg = rgb_color::black) const;
				
	template<typename Condition_func, typename Callback_func>
	void nearest_neighbors(std::size_t k, Condition_func cond, Callback_func callback, bool parallel = true) const {
		nearest_neighbors_(this, k, cond, callback, parallel);	
	}
	
	template<typename Condition_func, typename Callback_func>
	void nearest_neighbors(std::size_t k, Condition_func cond, Callback_func callback, bool parallel = true) {
		nearest_neighbors_(this, k, cond, callback, parallel);	
	}
	
	float ratio_of_area_filled() const;
};

using range_point_cloud_xyz = range_point_cloud<point_xyz>;
using range_point_cloud_full = range_point_cloud<point_full>;

extern template class range_point_cloud<point_xyz>;
extern template class range_point_cloud<point_full>;

}

#include "range_point_cloud.tcc"

#endif