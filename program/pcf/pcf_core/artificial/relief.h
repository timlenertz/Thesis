#ifndef PCF_RELIEF_POINT_CLOUD_H_
#define PCF_RELIEF_POINT_CLOUD_H_

#include "../point_cloud/unorganized/unorganized_point_cloud.h"
#include "../point_cloud/range/range_point_cloud.h"
#include "../point_cloud/range/camera_range_point_cloud.h"
#include "../util/multi_dimensional_array.h"
#include "../geometry/bounding_box.h"
#include "../geometry/angle.h"
#include "../camera/projection_image_camera.h"
#include <vector>
#include <string>
#include <Eigen/Eigen>
#include <utility>

namespace pcf {

class relief {
private:
	struct engraving {
		float x;
		float y;
		float radius;
		float depth;
		bool out;
	};
	std::vector<engraving> engravings_;
	float width_;
	float height_multiplier_;
	Eigen::Vector2f corners_[4];
	
	
	using vertices_t = std::vector<Eigen::Vector2f>;
	using face_t = std::array<std::ptrdiff_t, 3>;
	using faces_t = std::vector<face_t>;
	using mesh_t = std::pair<vertices_t, faces_t>;
	
	mesh_t generate_mesh_(float mesh_density) const;

public:
	explicit relief(float width, int seed = 0, float height = 0.2);
	
	void crop(float width = 0.13, int seed = 0);
	void uncrop();
	
	bool in_bounds(float x, float y) const;
	point_full at(float x, float y, bool check_bounds = true) const;

	range_point_cloud_full make_point_cloud(float density) const;
	std::vector<point_full> make_samples(std::size_t n) const;
	
	camera_range_point_cloud_full<projection_image_camera> make_projected_point_cloud(const projection_image_camera& cam, float mesh_density = 100, bool correct_coordinates = false, unsigned mesh_scale = 10) const;
	
	projection_image_camera camera_at_angle(angle circumference, float elevation, std::size_t image_width) const;
	
	void export_to_ply(const std::string&, float density) const;
};

}

#endif