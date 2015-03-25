#ifndef PCF_POINT_TRANSFORM_ITERATOR_H_
#define PCF_POINT_TRANSFORM_ITERATOR_H_

#include <iterator>
#include <Eigen/Geometry>
#include "../point.h"

namespace pcf {

/**
Point iterator that applies affine transformation to point.
Provides read-only access to the point.
*/
template<typename Point>
class point_transform_iterator :
public std::iterator<std::random_access_iterator_tag, Point> {
private:
	Eigen::Affine3f transformation_;
	Point* point_;
	
	mutable bool should_load_transformed_point_;
	mutable Point transformed_point_;
	
	void load_transformed_point_() const;
	
public:
	point_transform_iterator(Point*, const Eigen::Affine3f& = Eigen::Affine3f());
	point_transform_iterator(const point_transform_iterator&) = default;
	
	point_transform_iterator& operator=(const point_transform_iterator& it) {
		transformation_ = it.transformation_;
		point_ = it.point_;
		return *this;
	}
	
	bool operator==(const point_transform_iterator&) const;
	bool operator!=(const point_transform_iterator&) const;
	bool operator<(const point_transform_iterator&) const;
	bool operator<=(const point_transform_iterator&) const;
	bool operator>(const point_transform_iterator&) const;
	bool operator>=(const point_transform_iterator&) const;
	
	const Point& operator*() const;
	const Point* operator->() const;
	
	const Point& real_point() const { return *point_; }
	Point& real_point() { return *point_; }

	point_transform_iterator& operator++();
	point_transform_iterator& operator--();
	point_transform_iterator operator++(int);
	point_transform_iterator operator--(int);
	
	point_transform_iterator& operator+=(std::ptrdiff_t);
	point_transform_iterator& operator-=(std::ptrdiff_t);
	
	point_transform_iterator operator+(std::ptrdiff_t) const;
	point_transform_iterator operator-(std::ptrdiff_t) const;
	
	std::ptrdiff_t operator-(const point_transform_iterator&) const;
	
	Point& operator[](std::ptrdiff_t) const;	
};

extern template class point_transform_iterator<point_xyz>;
extern template class point_transform_iterator<point_full>;

}

#include "transform_iterator.tcc"

#endif