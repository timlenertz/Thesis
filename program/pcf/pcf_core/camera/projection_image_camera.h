#ifndef PCF_PROJECTION_IMAGE_CAMERA_H_
#define PCF_PROJECTION_IMAGE_CAMERA_H_

#include "projection_camera.h"
#include "image_camera.h"

namespace pcf {

struct plane;

/**
Projection camera which additionally handles mapping onto pixel grid.
Image pixel XY coordinates linearily map to coordinates on projected plane. This can model a photo camera. Two kinds of depth measure supported: distance to camera (depth), and z-coordinate after application of projection matrix (projected depth). Latter is easier to compute.
*/
class projection_image_camera : public projection_camera, public image_camera {
public:
	projection_image_camera() = default;
	projection_image_camera(const pose&, const projection_frustum&, std::size_t imw, std::size_t imh);
	projection_image_camera(const pose&, const projection_bounding_box&, std::size_t imw, std::size_t imh);
	projection_image_camera(const plane&, float w, float h, std::size_t imw, std::size_t imh = 0);
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
