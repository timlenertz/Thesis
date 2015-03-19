#ifndef PCF_FIELD_GAUSSIAN_WEIGHT_H_
#define PCF_FIELD_GAUSSIAN_WEIGHT_H_

#include <cmath>
#include <Eigen/Eigen>

namespace pcf {

class field_gaussian_weight {
private:
	const float denominator_;
	
public:
	explicit field_gaussian_weight(float standard_deviation = 1.0) :
		denominator_(2.0f * standard_deviation * standard_deviation) { }


	float operator()(const Eigen::Vector3f& a, const Eigen::Vector3f& b) const {
		float sq_distance = (a - b).squaredNorm();
		return std::exp(- sq_distance / denominator_);
	};
};

}

#endif