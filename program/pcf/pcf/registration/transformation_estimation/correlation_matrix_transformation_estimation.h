#ifndef PCF_CORRELATION_MATRIX_TRANSFORMATION_ESTIMATION_H_
#define PCF_CORRELATION_MATRIX_TRANSFORMATION_ESTIMATION_H_

#include <Eigen/Eigen>
#include <vector>
#include <utility>
#include "../correspondences/registration_correspondence.h"

namespace pcf {

class correlation_matrix_transformation_estimation {
protected:
	Eigen::Vector3f fixed_sum_ = Eigen::Vector3f::Zero();
	Eigen::Vector3f loose_sum_ = Eigen::Vector3f::Zero();
	std::size_t count_ = 0;
	std::vector<registration_correspondence> cors_;

protected:
	Eigen::Matrix3f correlation_matrix_() const;

public:
	correlation_matrix_transformation_estimation& operator<<(const registration_correspondence&);
};

}

#endif