namespace pcf {

template<typename Correspondences>
Eigen::Affine3f svd_transformation_estimation<Correspondences>::operator() () {
	super::compute_centers_();
	super::compute_correlation_matrix_();
	
	Eigen::JacobiSVD<Eigen::Matrix3f> svd(super::correlation_matrix_, Eigen::ComputeFullU | Eigen::ComputeFullV);
	Eigen::Matrix3f U = svd.matrixU();
	Eigen::Matrix3f V = svd.matrixV();
	if(U.determinant() * V.determinant() < 0) {
		for(std::ptrdiff_t i = 0; i < 3; ++i) V(i, 2) = -V(i, 2);
	}
	
	Eigen::Matrix3f R = V * U.transpose();
	Eigen::Vector3f t = super::loose_center_.head(3) - R * super::fixed_center_.head(3);
		
	return Eigen::Translation3f(t) * Eigen::AngleAxisf(R);
}


}