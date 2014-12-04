#ifndef PCF_POINT_TRANSFORM_ITERATOR_H_
#define PCF_POINT_TRANSFORM_ITERATOR_H_

#include <iterator>
#include <Eigen/Geometry>

namespace pcf {

/**
Point iterator that applies affine transformation to point.
Provides read-only access to the point.
*/
template<typename Point>
class point_transform_iterator :
public std::iterator<std::forward_iterator_tag, Point> {
private:
	const Eigen::Affine3f transformation_;
	Point* point_;
	
	mutable bool should_load_transformed_point_;
	mutable Point transformed_point_;
	
	void load_transformed_point_() const;
	
public:
	point_transform_iterator(const Eigen::Affine3f&, Point*);
	point_transform_iterator(const point_transform_iterator&) = default;
	
	point_transform_iterator& operator=(const point_transform_iterator&) = default;
	
	bool operator==(const point_transform_iterator&) const;
	bool operator!=(const point_transform_iterator&) const;
	
	const Point& operator*() const;
	const Point* operator->() const;

	point_transform_iterator& operator++();
	point_transform_iterator operator++(int);
};

}

#include "transform_iterator.tcc"

#endif