#ifndef PCF_POINT_CLOUD_H_
#define PCF_POINT_CLOUD_H_

namespace pcf {

/**

*/
template<typename Point>
class point_cloud {
protected:
	Point* const buffer_;
	Point* const buffer_end_; 
	
	point_cloud() : buffer_(nullptr), buffer_end(nullptr) { }
	
public:
	point_cloud(Point* buffer, Point* buffer_end) :
	buffer_(buffer), buffer_end_(buffer_end) { }
	virtual ~point_cloud() { }
	
	point_cloud(const point_cloud&) = delete;
	point_cloud& operator=(const point_cloud&) = delete;
	
	std::size_t size() const { return buffer_end_ - buffer_; }
	
	Point* data() { return buffer_; }
	const Point* data() const { return buffer_; }
	
	Point& operator[](std::ptrdiff_t i) { return buffer_[i]; }
	const Point& operator[](std::ptrdiff_t i) const { return buffer_[i]; }
	
	Point* begin() { return buffer_; }
	const Point* begin() const { return buffer_; }
	const Point* cbegin() const { return buffer_; }
	
	Point* end() { return buffer_end_; }
	const Point* end() const { return buffer_end_; }
	const Point* cend() const { return buffer_end_; }
};

}

#endif