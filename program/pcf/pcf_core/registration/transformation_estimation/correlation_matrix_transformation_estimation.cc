#include "correlation_matrix_transformation_estimation.h"

namespace pcf {

Eigen::Matrix3f correlation_matrix_transformation_estimation::correlation_matrix_() const {
	Eigen::Matrix3f correlation = Eigen::Matrix3f::Zero();
	Eigen::Vector3f fixed_center = fixed_center_();
	Eigen::Vector3f loose_center = loose_center_();
	
	#pragma omp parallel
	{
		Eigen::Matrix3f correlation_part = Eigen::Matrix3f::Zero();
		
		#pragma omp for
		for(auto it = cors_.begin(); it < cors_.end(); ++it) {
			Eigen::Matrix3f c = (it->fixed - fixed_center) * (it->loose - loose_center).transpose();
			c *= it->weight; // not sure if this is correct / if they need to be normalized
			correlation_part += c;
		}
		
		#pragma omp critical
		{
			correlation += correlation_part;
		}
	}
	
	correlation /= total_weight_;
	
	return correlation;
}


Eigen::Vector3f correlation_matrix_transformation_estimation::fixed_center_() const {
	return fixed_weighted_sum_ / total_weight_;
}

Eigen::Vector3f correlation_matrix_transformation_estimation::loose_center_() const {
	return loose_weighted_sum_ / total_weight_;
}


correlation_matrix_transformation_estimation& correlation_matrix_transformation_estimation::operator<<
(const registration_correspondence& cor) {
	total_weight_ += cor.weight;
	fixed_weighted_sum_ += cor.fixed * cor.weight;
	loose_weighted_sum_ += cor.loose * cor.weight;
	cors_.push_back(cor);
	return *this;
}



correlation_matrix_transformation_estimation& correlation_matrix_transformation_estimation::operator<<
(const correlation_matrix_transformation_estimation& est) {
	total_weight_ += est.total_weight_;
	fixed_weighted_sum_ += est.fixed_weighted_sum_;
	loose_weighted_sum_ += est.loose_weighted_sum_;
	cors_.insert(cors_.end(), est.cors_.begin(), est.cors_.end());
	return *this;
}



}