#include "default.h"
#include "../../pcf_core/util/random.h"

namespace pcf {

Eigen::Affine3f default_displacement::random_rotation_(float max_angle) {
	return Eigen::Affine3f(
		Eigen::AngleAxisf(random_real<float>(0, max_angle), Eigen::Vector3f::UnitX()) *
		Eigen::AngleAxisf(random_real<float>(0, max_angle), Eigen::Vector3f::UnitY()) *
		Eigen::AngleAxisf(random_real<float>(0, max_angle), Eigen::Vector3f::UnitZ())
	);
}


Eigen::Affine3f default_displacement::operator()(float arg) {
	Eigen::Affine3f translation = random_rotation_(two_pi) * Eigen::Translation3f(Eigen::Vector3f::UnitX());
	translation *= (arg * maximal_translation);
	Eigen::Affine3f rotation = random_rotation_(arg * (float)maximal_rotation_angle);
	return rotation * translation;
}

}