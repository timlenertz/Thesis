#ifndef PCF_MERGE_POINT_CLOUD_H_
#define PCF_MERGE_POINT_CLOUD_H_

#include "unorganized_point_cloud.h"
#include <initializer_list>
#include <utility>

namespace pcf {

template<typename Iterator, typename Point = typename Iterator::value_type::point_type>
unorganized_point_cloud<Point> merge_point_clouds(Iterator begin, Iterator end);

template<typename Point, typename Other_point>
unorganized_point_cloud<Point> operator+(const point_cloud<Point>&, const point_cloud<Other_point>&);

}

#include "merge.tcc"

#endif