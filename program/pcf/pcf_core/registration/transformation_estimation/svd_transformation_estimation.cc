#include "svd_transformation_estimation.h"

namespace pcf {

Eigen::Affine3f svd_transformation_estimation::operator() () const {
	Eigen::Vector3f fixed_center = fixed_center_();
	Eigen::Vector3f loose_center = loose_center_();
	Eigen::Matrix3f W = correlation_matrix_();
	
	Eigen::JacobiSVD<Eigen::Matrix3f> svd(W, Eigen::ComputeFullU | Eigen::ComputeFullV);
	Eigen::Matrix3f U = svd.matrixU();
	Eigen::Matrix3f V = svd.matrixV();
	if(U.determinant() * V.determinant() < 0) {
		for(std::ptrdiff_t i = 0; i < 3; ++i) V(i, 2) = -V(i, 2);
	}
	
	Eigen::Matrix3f R = V * U.transpose();
	Eigen::Vector3f t = loose_center - R * fixed_center;
		
	return Eigen::Translation3f(t) * Eigen::AngleAxisf(R);
}

}