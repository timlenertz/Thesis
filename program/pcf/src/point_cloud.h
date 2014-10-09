#ifndef PCF_POINT_CLOUD_H_
#define PCF_POINT_CLOUD_H_

#include <cstdint>
#include <stdexcept>
#include <utility>
#include <memory>
#include <limits>
#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include "asset.h"

namespace pcf {

template<typename Point, typename Allocator = std::allocator<Point>>
class point_cloud : public asset {
public:
	using point_type = Point;
	using iterator = Point*;
	using const_iterator = const Point*;

private:
	Allocator allocator_;

protected:
	Point* buffer_;
	Point* buffer_end_;
	
	void check_correct_alignment_() const {
		if((std::uintptr_t)buffer_ % alignof(Point))
			throw std::runtime_error("Point cloud data not properly aligned.");
	}

public:
	point_cloud(std::size_t size, const Allocator& alloc = Allocator()) :
	allocator_(alloc) {
		buffer_ = allocator_.allocate(size);
		buffer_end_ = buffer_ + size;
		check_correct_alignment_();
	}
	
	~point_cloud() {
		allocator_.deallocate(buffer_, size());
	}
	
	std::size_t size() const { return buffer_end_ - buffer_; }
	
	Point& operator[](std::ptrdiff_t d) { return buffer_[d]; }
	const Point& operator[](std::ptrdiff_t d) const { return buffer_[d]; }
	
	Point* data() { return buffer_; }
	const Point* data() const { return buffer_; }
	const Point* data_end() const { return buffer_end_; }
	
	iterator begin() { return buffer_; }
	const_iterator begin() const { return buffer_; }
	const_iterator cbegin() const { return buffer_; }
	iterator end() { return buffer_end_; }
	const_iterator end() const { return buffer_end_; }
	const_iterator cend() const { return buffer_end_; }
	
	template<typename Transformation>
	void apply_transformation(const Transformation& t) {
		const Eigen::Affine3f affinet(t);
		#pragma omp parallel for
		for(Point* p = buffer_; p < buffer_end_; ++p) p->apply_transformation(affinet);
	}
	
	Eigen::Vector3f mean() const {
		Eigen::Vector4f sum(0, 0, 0, 0);
		//#pragma omp parallel for shared(sum)
		for(Point* p = buffer_; p < buffer_end_; ++p) sum += p->homogeneous_coordinates();
		return Eigen::Vector3f(sum[0] / size(), sum[1] / size(), sum[2] / size());
	}
	
	template<typename Distance_func>
	Point& find_closest_point(const Point& from, const Distance_func& dist) {
		float minimal_distance = std::numeric_limits<float>::infinity();
		Point* closest_point = nullptr;
		
		for(Point* p = buffer_; p < buffer_end_; ++p) {
			float d = dist(*p, from);
			if(d < minimal_distance) {
				minimal_distance = d;
				closest_point = p;
			}
		}
		
		return *closest_point;
	}
};

}

#endif