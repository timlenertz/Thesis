#ifndef PCF_POINT_CLOUD_SELECTION_H_
#define PCF_POINT_CLOUD_SELECTION_H_

#include <vector>
#include "../util/dereference_iterator.h"

namespace pcf {

template<typename Point>
class point_cloud_selection {
private:
	std::vector<Point*> point_ptrs_;

public:
	using vector_type = std::vector<Point*>;

	using iterator = dereference_iterator<typename std::vector<Point*>::const_iterator>;

	point_cloud_selection() = default;
	point_cloud_selection(const vector_type& vec) : point_ptrs_(vec) { }
	
	void add(Point& pt) { point_ptrs_.push_back(&pt); }
	void clear() { point_ptrs_.clear(); }
	
	iterator begin() const { return iterator(point_ptrs_.begin()); }
	iterator end() const { return iterator(point_ptrs_.end()); }

	std::size_t size() const { return point_ptrs_.size(); }
	Point& operator[](std::ptrdiff_t i) const { return *point_ptrs_[i]; }
};

}

#endif
