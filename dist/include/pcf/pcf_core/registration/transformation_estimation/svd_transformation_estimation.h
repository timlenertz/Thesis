#ifndef PCF_SVD_TRANSFORMATION_ESTIMATION_H_
#define PCF_SVD_TRANSFORMATION_ESTIMATION_H_

#include "correlation_matrix_transformation_estimation.h"
#include "../error_metric/mean_square_error.h"
#include <Eigen/Eigen>
#include <Eigen/Geometry>

namespace pcf {

/**
Estimates transformation between correspondence pairs using single value decomposition.
*/
class svd_transformation_estimation : public correlation_matrix_transformation_estimation {
public:
	using error_metric = mean_square_error;

	svd_transformation_estimation() = default;
	Eigen::Affine3f operator() () const;
};

}

#endif
