#ifndef PCFEX_DEFAULT_DISPLACEMENT_H_
#define PCFEX_DEFAULT_DISPLACEMENT_H_

#include <Eigen/Geometry>

namespace pcf {

class space_object;

class default_displacement {
public:
	Eigen::Affine3f operator()(float arg);
};

}

#endif
