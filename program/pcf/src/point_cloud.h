#ifndef PCF_POINT_CLOUD_H_
#define PCF_POINT_CLOUD_H_

#include <cstdint>
#include <stdexcept>

namespace pcf {

template<typename Point>
class point_cloud {
public:
	using point_type = Point;
	
protected:
	Point* buffer_;
	Point* buffer_end_;
	
	point_cloud() : buffer_(nullptr), buffer_end_(nullptr) { }

	bool has_correct_alignment_() const { return ((std::uintptr_t)buffer_ % alignof(Point) == 0); }

public:
	using iterator = Point*;
	using const_iterator = const Point*;

	point_cloud(Point* buf, std::size_t size) :
	buffer_(buf), buffer_end_(buf + size) {
		if(! has_correct_alignment_()) throw std::runtime_error("Point cloud data not properly aligned.");
	}
	
	virtual ~point_cloud() { }
	
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
	
	template<typename Transform>
	void apply_transformation(const Transform& t) {
		#pragma omp parallel for
		for(Point* p = buffer_; p < buffer_end_; ++p) p->apply_transformation(t);
	}
};

}

#endif