#ifndef PCF_SVD_TRANSFORMATION_ESTIMATION_H_
#define PCF_SVD_TRANSFORMATION_ESTIMATION_H_

#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include "transformation_estimation.h"

namespace pcf {

template<typename Correspondences>
class svd_transformation_estimation : public transformation_estimation<Correspondences> {
	using super = transformation_estimation<Correspondences>;

public:
	explicit svd_transformation_estimation(const Correspondences& cor) :
		super(cor) { }
	
	Eigen::Affine3f operator() () const;
};

}

#include "svd_transformation_estimation.tcc"

#endif
