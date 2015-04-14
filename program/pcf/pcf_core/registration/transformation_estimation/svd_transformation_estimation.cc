#include "svd_transformation_estimation.h"

namespace pcf {

Eigen::Affine3f svd_transformation_estimation::operator() () const {
	if(total_weight_ == 0.0) return Eigen::Affine3f::Identity();

	Eigen::Vector3f fixed_center = fixed_center_();
	Eigen::Vector3f loose_center = loose_center_();
	Eigen::Matrix3f W = correlation_matrix_();
	
	Eigen::JacobiSVD<Eigen::Matrix3f> svd(W, Eigen::ComputeFullU | Eigen::ComputeFullV);
	Eigen::Matrix3f U = svd.matrixU();
	Eigen::Matrix3f V = svd.matrixV();
	
	Eigen::Matrix3f R = V * U.transpose();
	if(R.determinant() < 0) {
		for(std::ptrdiff_t i = 0; i < 3; ++i) R(i, 2) = -R(i, 2);
	}
	
	Eigen::Vector3f t = loose_center - R * fixed_center;
		
	return (Eigen::Translation3f(t) * Eigen::AngleAxisf(R)).inverse();
}

}