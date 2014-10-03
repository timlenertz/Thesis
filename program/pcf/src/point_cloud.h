#ifndef PCF_POINT_CLOUD_H_
#define PCF_POINT_CLOUD_H_

#include <cstdint>
#include <stdexcept>

namespace pcf {

template<typename Point>
class point_cloud {
public:
	using point_type = Point;

private:
	bool has_correct_alignment_() const { return ((std::uintptr_t)buffer_ % alignof(Point) == 0); }
	
protected:
	Point* const buffer_;
	Point* const buffer_end_;

public:
	using iterator = Point*;
	using const_iterator = const Point*;

	point_cloud(Point* buf, std::size_t size) :
	buffer_(buf), buffer_end_(buf + size) {
		if(! has_correct_alignment_()) throw std::runtime_error("Point cloud memory not properly aligned.");
	}
	
	virtual ~point_cloud() { }
	
	std::size_t size() const { return buffer_end_ - buffer_; }
	
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
	void apply_transformation(const Transform& T) {
		#pragma omp parallel for
		for(iterator it = begin(); it < end(); ++it) *it = T * *it;
	}
};

}

#endif