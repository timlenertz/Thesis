#include "iterative_correspondences_registration.h"

namespace pcf {

auto iterative_correspondences_registration_base::
run_and_get_statistics(const iteration_callback& cb) -> std::vector<iteration_state> {
	std::vector<iteration_state> stat;
	this->run([&stat](const iteration_state& st, bool done) {
		stat.push_back(st);
	});
	return stat;
}


void iterative_correspondences_registration_base::run(const iteration_callback& cb) {
	this->estimate_transformation();
	
	std::size_t iteration_count = 1;	
	while(iteration_count++ < maximal_iterations && error_ > minimal_error) {
		float previous_error = error_;
		
		if(cb) cb(iteration_state(current_transformation_, error_), false);
		
		Eigen::Affine3f previous_transformation = current_transformation_;
		this->estimate_transformation();

		if(error_ > previous_error && stop_on_divergence) {
			current_transformation_ = previous_transformation;
			break;
		}
	}
	if(cb) cb(iteration_state(current_transformation_, error_), true);
}



}