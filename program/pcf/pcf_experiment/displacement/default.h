#ifndef PCFEX_DEFAULT_DISPLACEMENT_H_
#define PCFEX_DEFAULT_DISPLACEMENT_H_

#include <Eigen/Geometry>
#include "../../pcf_core/geometry/angle.h"
#include "../../pcf_core/geometry/math_constants.h"

namespace pcf {

class space_object;

class default_displacement {
private:
	static Eigen::Affine3f random_rotation_(float max_angle);

public:
	float maximal_translation = 1.0;
	angle maximal_rotation_angle = 0.3 * pi;

	Eigen::Affine3f operator()(float arg);
};

}

#endif
