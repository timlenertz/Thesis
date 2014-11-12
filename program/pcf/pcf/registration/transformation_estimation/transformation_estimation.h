#ifndef PCF_TRANSFORMATION_ESTIMATION_H_
#define PCF_TRANSFORMATION_ESTIMATION_H_

namespace pcf {

template<typename Correspondences>
class transformation_estimation {
protected:
	const Correspondences& cors_;

	void compute_centers_();
	void compute_correlation_matrix_();
	
	Eigen::Vector3f fixed_center_;
	Eigen::Vector3f loose_center_;
	Eigen::Matrix3f correlation_matrix_;
	
	explicit transformation_estimation(const Correspondences&);
};

}

#include "transformation_estimation.tcc"

#endif