#ifndef PCF_RANGE_POINT_CLOUD_H_
#define PCF_RANGE_POINT_CLOUD_H_

#include <vector>
#include <limits>
#include "../../geometry/projection_camera.h"
#include "../../geometry/pose.h"
#include "../point_cloud.h"
#include "../../range_image.h"
#include "../../util/multi_dimensional_buffer.h"

namespace pcf {

class range_point_cloud_importer;

/**

*/
template<typename Point, typename Allocator = default_allocator<Point>>
class range_point_cloud : public point_cloud<Point, Allocator> {
	using super = point_cloud<Point, Allocator>;

private:
	const projection_camera camera_;
	multi_dimensional_buffer<Point, 2> image_;

	template<typename Other_cloud> void project_(const Other_cloud&);

public:
	template<typename Other_cloud>
	range_point_cloud(const Other_cloud&, const camera&, angle angular_res_x, angle angular_res_y = 0, const Allocator& = Allocator());

	range_point_cloud(const range_image&, const camera&, const Allocator& = Allocator());

	explicit range_point_cloud(range_point_cloud_importer&, const Allocator& = Allocator());

	const projection_camera& get_camera() const { return camera_; }
	
	std::size_t width() const;
	std::size_t height() const;
};

using range_point_cloud_xyz = range_point_cloud<point_xyz>;
using range_point_cloud_full = range_point_cloud<point_full>;

}

#include "range_point_cloud.tcc"

#endif