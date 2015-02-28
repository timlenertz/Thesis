#ifndef PCF_POINT_ALGORITHM_H_
#define PCF_POINT_ALGORITHM_H_

#include "point.h"

namespace pcf {

/**
Iterate through points and find closest point to reference point.
If any point is closer than \a accepting_distance, that one may be accepted. Point cloud classes implement more efficient algorithms for this based on their structure.
*/
template<typename Point, typename Iterator>
Iterator find_closest_point(const Point& ref, Iterator begin, Iterator end, float accepting_distance = 0);

/**
Sets all points to given color.
Iterator must yield point_full objects.
*/
template<typename Iterator>
void set_unique_color(Iterator begin, Iterator end, rgb_color);

}

#include "point_algorithm.tcc"

#endif