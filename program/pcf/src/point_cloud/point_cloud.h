#ifndef PCF_POINT_CLOUD_H_
#define PCF_POINT_CLOUD_H_

#include <cstdint>
#include <stdexcept>
#include <utility>
#include <memory>
#include <limits>
#include <cstring>
#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include "../asset.h"
#include "../point.h"

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
	Point* const buffer_;
	Point* buffer_end_;
	const std::size_t allocated_size_;
	
	void check_correct_alignment_() const {
		if((std::uintptr_t)buffer_ % alignof(Point))
			throw std::runtime_error("Point cloud data not properly aligned.");
	}

public:
	explicit point_cloud(std::size_t size, const Allocator& alloc = Allocator()) :
	allocator_(alloc), buffer_(allocator_.allocate(size)), buffer_end_(buffer_ + size), allocated_size_(size) {
		check_correct_alignment_();
	}
	
	
	template<typename Other_alloc>
	point_cloud(const point_cloud<Point, Other_alloc>& pc, const Allocator& alloc = Allocator()) :
	point_cloud(pc.capacity(), alloc) {
		std::memcpy((void*)buffer_, (const void*)pc.buffer_, pc.size()*sizeof(Point));
	}
	
	template<typename Other>
	point_cloud(const Other& pc, const Allocator& alloc = Allocator()) :
	point_cloud(pc.capacity(), alloc) {
		Point* o = buffer_;
		
		#pragma omp parallel for
		for(typename Other::const_iterator i = pc.cbegin(); i < pc.cend(); ++i) *(o++) = *i;
	}
	
	std::size_t size() const { return buffer_end_ - buffer_; }
	std::size_t capacity() const { return allocated_size_; }

	void resize(std::size_t new_size) {
		if(new_size > capacity()) throw std::logic_error("Point cloud new size cannot be greater than capacity.");
		else buffer_end_ = buffer_ + new_size;
	}
	
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
		
		#pragma omp parallel
		{
			Eigen::Vector4f s(0, 0, 0, 0);
			#pragma omp for
			for(Point* p = buffer_; p < buffer_end_; ++p) s += p->homogeneous_coordinates;
			
			#pragma omp critical
			{ sum += s; }
		}

		return Eigen::Vector3f(sum[0] / size(), sum[1] / size(), sum[2] / size());
	}
	
	template<typename Other_point, typename Distance_func>
	Point& find_closest_point(const Other_point& from, Distance_func dist) {
		float minimal_distance = std::numeric_limits<float>::infinity();
		Point* closest_point = nullptr;
		
		#pragma omp parallel
		{
			float min_d = std::numeric_limits<float>::infinity();
			Point* cp = nullptr;

			#pragma omp for
			for(Point* p = buffer_; p < buffer_end_; ++p) {
				float d = dist(*p, from);
				if(d < min_d) { min_d = d; cp = p; }
			}

			#pragma omp critical
			if(min_d < minimal_distance) { minimal_distance = min_d; closest_point = cp; }
		}
		
		return *closest_point;
	}
	
	void remove_invalid_points() {
		Point* np = buffer_;
		for(Point* p = buffer_; p < buffer_end_; ++p) if(p->valid()) *(np++) = *p;
		buffer_end_ = np;
	}
};

}

#endif