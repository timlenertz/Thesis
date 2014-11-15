#include <iostream>
#include <cmath>

namespace pcf {

template<typename Cloud_fixed, typename Cloud_loose, typename Correspondences, typename Transformation_estimation, typename Error_metric>
void iterative_correspondences_registration<Cloud_fixed, Cloud_loose, Correspondences, Transformation_estimation, Error_metric>::iteration() {
	receiver rec;

	Transformation_estimation transformation_estimation;
	Error_metric error_metric;
	
	correspondences_.find_correspondences_parallel(rec);
	
	error_ = rec.error_metric();
	estimated_transformation_ = rec.transformation_estimation().inverse();
	
	std::cout << error_ << std::endl;
}


template<typename Cloud_fixed, typename Cloud_loose, typename Correspondences, typename Transformation_estimation, typename Error_metric>
void iterative_correspondences_registration<Cloud_fixed, Cloud_loose, Correspondences, Transformation_estimation, Error_metric>::run() {
	iteration();
	
	std::size_t iteration_count = 1;	
	while(iteration_count++ < maximal_iterations_ && error_ > minimal_error_) {
		float previous_error = error_;
		Eigen::Affine3f previous_estimated_transformation = estimated_transformation_;
	
		loose_.apply_transformation(estimated_transformation_);
		iteration();
		
		if(error_ > previous_error) {
			loose_.apply_transformation(previous_estimated_transformation.inverse());
			break;
		}
		
		previous_estimated_transformation = estimated_transformation_;
		previous_error = error_;
	}
}


}
