#include "projection_camera.h"

namespace pcf {

projection_camera(const pose& ps, const projection_frustum& fr, std::size_t imw) :
	camera(ps, fr),
	image_width(imw),
	image_height(imw / fr.aspect_ratio()) { }


projection_camera(const camera& cam, std::size_t imw) :
	projection_camera(cam.pose_, cam.frustum_) { }

	
float aspect_ratio() const {
	return float(image_width) / image_height;
}

	
image_coordinates to_image(const Eigen::Vector3f& p) const {

}


spherical_coordinates image_to_spherical(const image_coordinates& ic, float distance) const {

}


Eigen::Vector3f point(const image_coordinates& ic, float z) const {

}	


}
