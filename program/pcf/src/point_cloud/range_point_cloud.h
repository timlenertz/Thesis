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
	Eigen::Projective3f projection_matrix_;

	std::ptrdiff_t offset_(std::ptrdiff_t x, std::ptrdiff_t y) const {
		return y*width_ + x;
	}

public:
	range_point_cloud(std::size_t w, std::size_t h, const Allocator& alloc = Allocator()) :
	super(w * h, alloc), width_(w), height_(h) { }

	std::size_t width() const { return width_; }
	std::size_t height() const { return height_; }
	
	Point& at(std::ptrdiff_t x, std::ptrdiff_t y) { return super::buffer_[ offset_(x, y) ]; }
	
	template<typename Transformation>
	void apply_transformation(const Transformation& t) {
		super::apply_transformation(t);
		projection_matrix_ *= t.inverse();
	}
	
	void erase() {
		#pragma omp parallel for
		for(Point* p = super::buffer_; p < super::buffer_end_; ++p) p->invalidate();
	}
	
	template<typename Other_cloud>
	void project_point_cloud(const Other_cloud& pc, const Eigen::Projective3f& projm) {
		projection_matrix_ = projm;
		std::vector<float> z_buffer(width_ * height_, std::numeric_limits<float>::infinity());
		
		#pragma omp parallel for
		for(auto p = pc.cbegin(); p < pc.cend(); ++p) {
			Eigen::Vector4f proj_h = projm * p->homogeneous_coordinates;
			Eigen::Vector3f proj = proj_h.block(0, 0, 3, 1) / proj_h[3];
			
			float z = proj[2], old_z;
			if(z < 0.0) continue;

			std::ptrdiff_t x = 0.5f*(proj[0] + 1.0) * width_;
			if(x < 0 || x >= (std::ptrdiff_t)width_) continue;
			
			std::ptrdiff_t y = 0.5f*(proj[1] + 1.0) * height_;
			if(y < 0 || y >= (std::ptrdiff_t)height_) continue;

			std::ptrdiff_t off = offset_(x, y);
			
			#pragma omp atomic read
			old_z = z_buffer[off];
			
			if(z < old_z) {			
				#pragma omp critical
				{ z_buffer[off] = z; super::buffer_[off] = *p; }
			}
		}
	}
	

	

	void print() {
		std::vector<float> img(width_ * height_, -1.0);
		float mn = 1000000, mx = -1000000;
		for(std::ptrdiff_t y = 0; y < height_; ++y) {
			for(std::ptrdiff_t x = 0; x < width_; ++x) {
				const auto& p = at(x, y);
				if(! p) continue;
				auto pr = projection_matrix_ * p.homogeneous_coordinates;
				float z = pr[2] / pr[3];
				if(z < mn) mn = z;
				if(z > mx) mx = z;
				img[offset_(x, y)] = z;
			}
		}

	
		for(std::ptrdiff_t y = 0; y < height_; ++y) {
			for(std::ptrdiff_t x = 0; x < width_; ++x) {
				float z = img[offset_(x, y)];
				int r = 5 * (z - mn)/(mx - mn);
				
				switch(r) {
				case 0: std::cout << '#'; break;
				case 1: std::cout << 'X'; break;
				case 2: std::cout << 'x'; break;
				case 3: std::cout << ':'; break;
				case 4: std::cout << '.'; break;
				default: std::cout << ' ';
				}
			}
			std::cout << std::endl;
		}
	}
};

}

#endif