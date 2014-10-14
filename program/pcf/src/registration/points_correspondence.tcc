namespace pcf {

template<typename Cloud_fixed, typename Cloud_loose> template<typename Distance_func>
float points_correspondence<Cloud_fixed, Cloud_loose>::error(Distance_func dist) const {
	float sum = 0.0;
	#pragma omp parallel for reduction(+:sum)
	for(auto it = correspondences_.cbegin(); it < correspondences_.cend(); ++it) {
		sum += it->weight * dist(it->fixed, it->loose);
	}
	return sum;
}


template<typename Cloud_fixed, typename Cloud_loose>
void points_correspondence<Cloud_fixed, Cloud_loose>::centers_of_mass_(Eigen::Vector4f& fixed, Eigen::Vector4f& loose) const {
	fixed.setZero();
	loose.setZero();

	#pragma omp parallel
	{
		Eigen::Vector4f fixed_sum_part = Eigen::Vector4f::Zero();
		Eigen::Vector4f loose_sum_part = Eigen::Vector4f::Zero();
	
		#pragma omp for
		for(auto it = correspondences_.cbegin(); it < correspondences_.cend(); ++it) {
			fixed_sum_part += it->fixed.homogeneous_coordinates;
			loose_sum_part += it->loose.homogeneous_coordinates;
		}
	
		#pragma omp critical
		{
			fixed += fixed_sum_part;
			loose += loose_sum_part;
		}
	}
	
	fixed /= size();
	loose /= size();
}

template<typename Cloud_fixed, typename Cloud_loose>
Eigen::Matrix3f points_correspondence<Cloud_fixed, Cloud_loose>::correlation_matrix_(const Eigen::Vector4f& fixed_center, const Eigen::Vector4f& loose_center) const {
	Eigen::Matrix4f correlation = Eigen::Matrix4f::Zero();
	#pragma omp parallel
	{
		Eigen::Matrix4f correlation_part = Eigen::Matrix4f::Zero();
		#pragma omp for
		for(auto it = correspondences_.cbegin(); it < correspondences_.cend(); ++it) {
			correlation_part +=
				(it->fixed.homogeneous_coordinates - fixed_center)
			  * (it->loose.homogeneous_coordinates - loose_center).transpose();
		}
		
		#pragma omp critical
		{ correlation += correlation_part; }
	}
	
	return correlation.block(0, 0, 3, 3);
}

template<typename Cloud_fixed, typename Cloud_loose>
Eigen::Affine3f points_correspondence<Cloud_fixed, Cloud_loose>::estimate_transformation_svd() const {
	Eigen::Vector4f fixed_center, loose_center;
	centers_of_mass_(fixed_center, loose_center);

	Eigen::Matrix3f W = correlation_matrix_(fixed_center, loose_center);
	
	Eigen::JacobiSVD<Eigen::Matrix3f> svd(W, Eigen::ComputeFullU | Eigen::ComputeFullV);
	Eigen::Matrix3f U = svd.matrixU();
	Eigen::Matrix3f V = svd.matrixV();
	if(U.determinant() * V.determinant() < 0) {
		for(std::ptrdiff_t i = 0; i < 3; ++i ) V(i, 2) = -V(i, 2);
	}
	
	Eigen::Matrix3f R = V * U.transpose();
	Eigen::Vector3f t = loose_center.head(3) - R * fixed_center.head(3);
		
	return Eigen::Translation3f(t) * Eigen::AngleAxisf(R);
}


}