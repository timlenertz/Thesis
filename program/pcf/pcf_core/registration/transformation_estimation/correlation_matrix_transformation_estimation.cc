#include "correlation_matrix_transformation_estimation.h"

namespace pcf {

Eigen::Matrix3f correlation_matrix_transformation_estimation::correlation_matrix_() const {
	// TODO weight

	Eigen::Matrix3f correlation = Eigen::Matrix3f::Zero();
	Eigen::Vector3f fixed_center = fixed_sum_ / count_;
	Eigen::Vector3f loose_center = loose_sum_ / count_;
	
	#pragma omp parallel
	{
		Eigen::Matrix3f correlation_part = Eigen::Matrix3f::Zero();
		
		#pragma omp for
		for(auto it = cors_.begin(); it < cors_.end(); ++it) {
			Eigen::Matrix3f c = (it->fixed - fixed_center) * (it->loose - loose_center).transpose();
			c *= it->weight;
			correlation_part += c;
		}
		
		#pragma omp critical
		{
			correlation += correlation_part;
		}
	}
	
	return correlation;
}


correlation_matrix_transformation_estimation& correlation_matrix_transformation_estimation::operator<<
(const registration_correspondence& cor) {
	++count_;
	fixed_sum_ += cor.fixed;
	loose_sum_ += cor.loose;
	cors_.push_back(cor);
	return *this;
}



correlation_matrix_transformation_estimation& correlation_matrix_transformation_estimation::operator<<
(const correlation_matrix_transformation_estimation& est) {
	count_ += est.count_;
	fixed_sum_ += est.fixed_sum_;
	loose_sum_ += est.loose_sum_;
	cors_.insert(cors_.end(), est.cors_.begin(), est.cors_.end());
	return *this;
}



}