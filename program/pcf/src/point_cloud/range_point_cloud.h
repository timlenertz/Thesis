#ifndef PCF_RANGE_POINT_CLOUD_H_
#define PCF_RANGE_POINT_CLOUD_H_

#include <iostream>
#include <vector>
#include <limits>
#include "point_cloud.h"

namespace pcf {

template<typename Point, typename Allocator = std::allocator<Point>>
class range_point_cloud : public point_cloud<Point, Allocator> {
private:
	using super = point_cloud<Point, Allocator>;
	
	const std::size_t width_, height_;
	Eigen::Matrix4f projection_matrix_;

	std::ptrdiff_t offset_(std::ptrdiff_t x, std::ptrdiff_t y) const {
		return y*width_ + x;
	}

public:
	range_point_cloud(std::size_t w, std::size_t h, const Eigen::Matrix4f& projm, const Allocator& alloc = Allocator()) :
	super(w * h, alloc), width_(w), height_(h), projection_matrix_(projm) { }

	std::size_t width() const { return width_; }
	std::size_t height() const { return height_; }
	
	Point& at(std::ptrdiff_t x, std::ptrdiff_t y) { return super::buffer_[ offset_(x, y) ]; }
	
	template<typename Transformation>
	void apply_transformation(const Transformation& t) {
		super::apply_transformation(t);
		projection_matrix_ = t * projection_matrix_;
	}
	
	void erase() {
		#pragma omp parallel for
		for(Point* p = super::buffer_; p < super::buffer_end_; ++p) p->invalidate();
	}
	
	template<typename Other_cloud>
	void project_point_cloud(const Other_cloud& pc, const Eigen::Matrix4f& projm) {
		std::vector<float> z_buffer(width_ * height_, std::numeric_limits<float>::infinity());
		
		for(const auto& p : pc) {
			Eigen::Vector4f proj_h = projm * p.homogeneous_coordinates;
			Eigen::Vector3f proj = proj_h.block(0, 0, 3, 1) / proj_h[3];
			
			float z = proj[2];
			if(z < 0.0) continue;

			std::ptrdiff_t x = 0.5f*(proj[0] + 1.0) * width_;
			if(x < 0 || x >= width_) continue;
			
			std::ptrdiff_t y = 0.5f*(proj[1] + 1.0) * height_;
			if(y < 0 || y >= height_) continue;

			std::ptrdiff_t off = offset_(x, y);
			float& old_z = z_buffer[off];
			if(z < old_z) {
				super::buffer_[off] = p;
				old_z = z;
			}
		}
	}
	
	void print() {
		for(std::ptrdiff_t y = 0; y < height_; ++y) {
			for(std::ptrdiff_t x = 0; x < width_; ++x) {
				if(at(x, y).valid()) std::cout << 'x';
				else std::cout << ' ';
			}
			std::cout << std::endl;
		}
	}
};

}

#endif