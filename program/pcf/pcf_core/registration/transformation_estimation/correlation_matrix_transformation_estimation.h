#ifndef PCF_CORRELATION_MATRIX_TRANSFORMATION_ESTIMATION_H_
#define PCF_CORRELATION_MATRIX_TRANSFORMATION_ESTIMATION_H_

#include <Eigen/Eigen>
#include <vector>
#include <utility>
#include "../correspondences/registration_correspondence.h"

namespace pcf {

/**
Transformation estimation which uses correlation matrix.
*/
class correlation_matrix_transformation_estimation {
protected:
	Eigen::Vector3f fixed_weighted_sum_ = Eigen::Vector3f::Zero();
	Eigen::Vector3f loose_weighted_sum_ = Eigen::Vector3f::Zero();
	float total_weight_;
	std::vector<registration_correspondence> cors_;

protected:
	correlation_matrix_transformation_estimation() = default;
	Eigen::Matrix3f correlation_matrix_() const;
	
	Eigen::Vector3f fixed_center_() const;
	Eigen::Vector3f loose_center_() const;

public:
	correlation_matrix_transformation_estimation& operator<<(const registration_correspondence&);
	correlation_matrix_transformation_estimation& operator<<(const correlation_matrix_transformation_estimation&);
};

}

#endif