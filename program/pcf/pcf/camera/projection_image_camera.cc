#include "projection_image_camera.h"
#include <stdexcept>

namespace pcf {

projection_image_camera::projection_image_camera(const pose& ps, const projection_frustum& fr, std::size_t imw, std::size_t imh) :
	projection_camera(ps, fr),
	image_camera(imw, imh) { }
	

projection_image_camera::projection_image_camera(const projection_camera& cam, std::size_t imw, std::size_t imh) :
	projection_camera(cam),
	image_camera(imw, imh) { }



auto projection_image_camera::to_image(const Eigen::Vector3f& p, float& z) const -> image_coordinates {
	Eigen::Vector2f proj = to_projected(p, z);
	std::ptrdiff_t x = ((proj[0] + 1.0f) * image_width_) / 2.0f;
	std::ptrdiff_t y = ((proj[1] + 1.0f) * image_height_) / 2.0f;
	return image_coordinates({x, y});
}


void projection_image_camera::adjust_field_of_view_x() {		
	frustum_.adjust_fov_x_to_aspect_ratio( image_aspect_ratio() );
}


void projection_image_camera::adjust_field_of_view_y() {		
	frustum_.adjust_fov_y_to_aspect_ratio( image_aspect_ratio() );
}


Eigen::Vector3f projection_image_camera::point(image_coordinates, float proj_depth) const {

}


}