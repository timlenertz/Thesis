#ifndef PCFEX_CONSTANT_DISPLACER_H_
#define PCFEX_CONSTANT_DISPLACER_H_

#include <Eigen/Geometry>
#include "../../pcf_core/geometry/pose.h"

namespace pcf {
namespace exper {

class constant_displacer {
public:
	Eigen::Affine3f transformation;
	
	explicit constant_displacer(const Eigen::Affine3f& t = Eigen::Affine3f::Identity()) :
		transformation(t) { }
	explicit constant_displacer(const pose& ps) :
		transformation(ps.transformation_from_world()) { }

	Eigen::Affine3f operator()(float arg)
		{ return transformation; }
};

}
}

#endif
