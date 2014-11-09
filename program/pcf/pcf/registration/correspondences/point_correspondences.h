#ifndef PCF_POINT_CORRESPONDENCES_H_
#define PCF_POINT_CORRESPONDENCES_H_

#include <functional>
#include "registration_correspondences.h"

namespace pcf {

/**
Correspondences between points from two point clouds.
Holds references to the points.
*/
template<typename Cloud_fixed, typename Cloud_loose>
using point_correspondences = registration_correspondences<
	std::reference_wrapper<const typename Cloud_fixed::point_type>,
	std::reference_wrapper<const typename Cloud_loose::point_type>
>;

}