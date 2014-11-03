#ifndef PCF_POINT_ALGORITHM_H_
#define PCF_POINT_ALGORITHM_H_

#include "point.h"

namespace pcf {

template<typename Point, typename Iterator>
Iterator find_closest_point(const Point& ref, Iterator begin, Iterator end);



}

#include "point_algorithm.tcc"

#endif