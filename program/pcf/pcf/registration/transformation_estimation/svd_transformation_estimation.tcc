namespace pcf {

template<typename Correspondences>
Eigen::Affine3f svd_transformation_estimation(const Correspondences& cor) {
	Eigen::Vector4f fixed_center, loose_center;
	centers_of_mass_(fixed_center, loose_center);

	Eigen::Matrix3f W = correlation_matrix_(fixed_center, loose_center);
	
	Eigen::JacobiSVD<Eigen::Matrix3f> svd(W, Eigen::ComputeFullU | Eigen::ComputeFullV);
	Eigen::Matrix3f U = svd.matrixU();
	Eigen::Matrix3f V = svd.matrixV();
	if(U.determinant() * V.determinant() < 0) {
		for(std::ptrdiff_t i = 0; i < 3; ++i) V(i, 2) = -V(i, 2);
	}
	
	Eigen::Matrix3f R = V * U.transpose();
	Eigen::Vector3f t = loose_center.head(3) - R * fixed_center.head(3);
		
	return Eigen::Translation3f(t) * Eigen::AngleAxisf(R);
}


}