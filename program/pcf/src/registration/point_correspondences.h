#ifndef PCF_POINT_CORRESPONDENCES_H_
#define PCF_POINT_CORRESPONDENCES_H_

#include <vector>
#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include "../point.h"

namespace pcf {

template<typename... Point>
class point_correspondences {
public:
	constexpr static std::size_t number_of_point_clouds = sizeof...(Point);

	struct correspondence {
		std::tuple<Point&...> points;
		float weight = 1.0;
	};

public:
	std::vector<correspondence> correspondences_;
	
	Eigen::Affine3f estimate_transformation();
};

}

#endif