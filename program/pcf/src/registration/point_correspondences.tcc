#include <cassert>

namespace pcf {

template<typename Cloud_fixed, typename Cloud_loose>
point_correspondences<Cloud_fixed, Cloud_loose>::point_correspondences(const Cloud_fixed& cf, Cloud_loose& cl) :
fixed_cloud_(cf), loose_cloud_(cl) { }


template<typename Cloud_fixed, typename Cloud_loose> template<typename Distance_func>
float point_correspondences<Cloud_fixed, Cloud_loose>::error(Distance_func dist) const {
	float sum = 0.0;
	#pragma omp parallel for reduction(+:sum)
	for(auto it = cors_.cbegin(); it < cors_.cend(); ++it) {
		sum += dist( it->fixed_point(), it->loose_point() );
	}
	return sum;
}


template<typename Cloud_fixed, typename Cloud_loose>
void point_correspondences<Cloud_fixed, Cloud_loose>::centers_of_mass_(Eigen::Vector4f& fixed, Eigen::Vector4f& loose) const {
	fixed.setZero();
	loose.setZero();

	#pragma omp parallel
	{
		Eigen::Vector4f fixed_sum_part = Eigen::Vector4f::Zero();
		Eigen::Vector4f loose_sum_part = Eigen::Vector4f::Zero();
	
		#pragma omp for
		for(auto it = cors_.cbegin(); it < cors_.cend(); ++it) {
			fixed_sum_part += it->fixed_point().homogeneous_coordinates;
			loose_sum_part += it->loose_point().homogeneous_coordinates;
		}
	
		#pragma omp critical
		{
			fixed += fixed_sum_part;
			loose += loose_sum_part;
		}
	}
	
	fixed /= cors_.size();
	loose /= cors_.size();
}

template<typename Cloud_fixed, typename Cloud_loose>
Eigen::Matrix3f point_correspondences<Cloud_fixed, Cloud_loose>::correlation_matrix_(const Eigen::Vector4f& fixed_center, const Eigen::Vector4f& loose_center) const {
	Eigen::Matrix4f correlation = Eigen::Matrix4f::Zero();
	#pragma omp parallel
	{
		Eigen::Matrix4f correlation_part = Eigen::Matrix4f::Zero();
		#pragma omp for
		for(auto it = cors_.cbegin(); it < cors_.cend(); ++it) {
			correlation_part +=
				(it->fixed_point().homogeneous_coordinates - fixed_center)
			  * (it->loose_point().homogeneous_coordinates - loose_center).transpose();
		}
		
		#pragma omp critical
		{ correlation += correlation_part; }
	}
	
	return correlation.block(0, 0, 3, 3);
}


template<typename Cloud_fixed, typename Cloud_loose>
Eigen::Affine3f point_correspondences<Cloud_fixed, Cloud_loose>::estimate_transformation_svd() const {
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