#include "iterative_correspondences_registration.h"

namespace pcf {

bool iterative_correspondences_registration_base::run(const iteration_callback& cb) {
	auto callback = [&]() {
		if(cb) cb(current_transformation_, error_);
	};

	this->estimate_transformation();
	
	std::size_t iteration_count = 1;	
	while(iteration_count++ < maximal_iterations && error_ > minimal_error) {
		float previous_error = error_;
		
		callback();
		
		Eigen::Affine3f previous_transformation = current_transformation_;
		this->estimate_transformation();

		if(error_ > previous_error && stop_on_divergence) {
			current_transformation_ = previous_transformation;
			error_ = previous_error;
			return true;
		}
	}
	
	callback();
	
	return (error_ <= minimal_error);
}



}