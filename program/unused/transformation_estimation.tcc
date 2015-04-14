namespace pcf {

template<typename Correspondences>
transformation_estimation<Correspondences>::transformation_estimation(const Correspondences& cors) :
cors_(cors) { }



template<typename Correspondences>
void transformation_estimation<Correspondences>::compute_centers_() {
	fixed_center_.setZero();
	loose_center_.setZero();
	
	#pragma omp parallel
	{
		Eigen::Vector3f fixed_sum_part = Eigen::Vector3f::Zero();
		Eigen::Vector3f loose_sum_part = Eigen::Vector3f::Zero();
		
		#pragma omp for
		for(auto it = cors_.begin(); it < cors_.end(); ++it) {
			fixed_sum_part += (Eigen::Vector3f) it->fixed();
			loose_sum_part += (Eigen::Vector3f) it->loose();
		}
		
		#pragma omp critical
		{
			fixed_center_ += fixed_sum_part;
			loose_center_ += loose_sum_part;
		}
	}
	
	fixed_center_ /= cors_.size();
	loose_center_ /= cors_.size();
}



template<typename Correspondences>
void transformation_estimation<Correspondences>::compute_correlation_matrix_() {
	correlation_matrix_.setZero();
	
	#pragma omp parallel
	{
		Eigen::Matrix3f correlation_part = Eigen::Matrix3f::Zero();
		
		#pragma omp for
		for(auto it = cors_.begin(); it < cors_.end(); ++it) {
			correlation_part +=
				((Eigen::Vector3f)it->fixed() - fixed_center_)
			  * ((Eigen::Vector3f)it->loose() - loose_center_).transpose();
		}
		
		#pragma omp critical
		{
			correlation_matrix_ += correlation_part;
		}
	}
}


}
