#ifndef PCF_DISKS_POINT_CLOUD_H_
#define PCF_DISKS_POINT_CLOUD_H_

#include "../point_cloud/unorganized/unorganized_point_cloud.h"
#include "../point_cloud/range/range_point_cloud.h"
#include "../point_cloud/range/camera_range_point_cloud.h"
#include "../camera/projection_image_camera.h"
#include "../geometry/plane.h"
#include <vector>
#include <Eigen/Geometry>
#include <utility>

namespace pcf {

class disks {
private:
	struct disk {
		Eigen::Vector3f o;
		Eigen::Vector3f n;
		float r;
	};

	std::vector<disk> disks_;
	float width_;	
	
public:
	explicit disks(float width, unsigned num = 10, int seed = 0);

	unorganized_point_cloud_full make_point_cloud(float density) const;
	unorganized_point_cloud_full make_point_cloud(float density, std::ptrdiff_t first, std::ptrdiff_t last) const;

	camera_range_point_cloud_full<projection_image_camera> make_projected_point_cloud(const projection_image_camera& cam) const;
	
	unorganized_point_cloud_full make_projected_point_cloud_without_occlusion(const projection_image_camera& cam) const;
};

}

#endif