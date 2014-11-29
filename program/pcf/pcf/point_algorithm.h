#ifndef PCF_POINT_ALGORITHM_H_
#define PCF_POINT_ALGORITHM_H_

#include "point.h"

namespace pcf {

template<typename Point, typename Iterator>
Iterator find_closest_point(const Point& ref, Iterator begin, Iterator end, float accepting_distance = 0);

template<typename Iterator>
void set_unique_color(Iterator begin, Iterator end, rgb_color);

}

#include "point_algorithm.tcc"

#endif