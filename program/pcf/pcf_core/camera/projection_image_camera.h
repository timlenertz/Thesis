#ifndef PCF_PROJECTION_IMAGE_CAMERA_H_
#define PCF_PROJECTION_IMAGE_CAMERA_H_

#include "projection_camera.h"
#include "image_camera.h"

namespace pcf {

class projection_image_camera : public projection_camera, public image_camera {
public:
	projection_image_camera() = default;
	projection_image_camera(const pose&, const projection_frustum&, std::size_t imw, std::size_t imh);
	projection_image_camera(const projection_camera&, std::size_t imw, std::size_t imh);
	projection_image_camera(const projection_image_camera&) = default;
	
	Eigen::Vector3f point_with_projected_depth(image_coordinates, float proj_depth) const;
	
	image_coordinates to_image(const Eigen::Vector3f&) const override;
	Eigen::Vector3f point(image_coordinates, float depth) const override;
	
	void adjust_field_of_view_x();
	void adjust_field_of_view_y();
};

}

#endif
