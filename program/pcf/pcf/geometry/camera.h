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
	camera(pose p, const Eigen::Projective3f& proj, std::size_t iw, std::size_t ih);
	camera(const camera&) = default;
	
	const pose& get_pose() const { return pose_; }
	const Eigen::Projective3f& get_projection() const { return projection_; }
	std::size_t get_image_width() const { return image_width_; }
	std::size_t get_image_height() const { return image_height_; }
	
	void set_pose(const pose&);
	void set_projection(const Eigen::Projective3f&);
	void set_image_size_(std::size_t w, std::size_t h);
		
	template<typename Point> image_coordinates project(const Point&) const;
	
	template<typename Point> float range(const Point&) const;
	template<typename Point> float depth(const Point&) const;
};

}

#endif
