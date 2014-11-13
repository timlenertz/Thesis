#ifndef PCF_SVD_TRANSFORMATION_ESTIMATION_H_
#define PCF_SVD_TRANSFORMATION_ESTIMATION_H_

#include "correlation_matrix_transformation_estimation.h"
#include <Eigen/Eigen>
#include <Eigen/Geometry>

namespace pcf {

class svd_transformation_estimation : public correlation_matrix_transformation_estimation {
public:	
	Eigen::Affine3f operator() () const;
};

}

#endif
