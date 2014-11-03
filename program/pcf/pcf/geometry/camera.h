#ifndef PCF_CAMERA_H_
#define PCF_CAMERA_H_

#include <Eigen/Geometry>
#include <array>
#include "pose.h"

namespace pcf {

class camera {
public:
	using image_coordinates = std::array<std::ptrdiff_t, 2>;
	
private:
	pose pose_;
	Eigen::Projective3f projection_;
	std::size_t image_width_;
	std::size_t image_height_;
	
	Eigen::Affine3f view_;
	Eigen::Projective3f view_projection_;
	image_coordinates image_center_;

	void update_();

public:
	camera(const pose& p, float fov, float znear, float zfar, std::size_t iw, std::size_t ih);
	camera(const pose& p, const Eigen::Projective3f& proj, std::size_t iw, std::size_t ih);
	camera(const camera&) = default;
	
	const pose& get_pose() const { return pose_; }
	const Eigen::Projective3f& get_projection() const { return projection_; }
	std::size_t get_image_width() const { return image_width_; }
	std::size_t get_image_height() const { return image_height_; }
		
	void set_pose(const pose&);
	void set_projection(const Eigen::Projective3f&);
	void set_perspective_projection(float fov, float znear, float zfar);
	void set_image_size(std::size_t w, std::size_t h);
	
	const Eigen::Affine3f& view_transformation() const { return view_; }
	const Eigen::Projective3f& view_projection_transformation() const { return view_projection_; }
	float aspect_ratio() const;
	
	std::size_t number_of_pixels() const { return image_width_ * image_height_; }
		
	image_coordinates project(const Eigen::Vector3f&) const;
	bool in_frustum(const Eigen::Vector3f&) const;
	
	float range(const Eigen::Vector3f&) const;
	float depth(const Eigen::Vector3f&) const;
};

}

#endif
