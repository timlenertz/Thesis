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
		pose previous_pose = this->get_pose_();
		float previous_error = error_;

		this->apply_estimated_transformation();
		
		if(cb) cb(iteration_state(estimated_transformation_, error_), false);
		
		this->estimate_transformation();
		
		if(error_ > previous_error && stop_on_divergence) {
			this->set_pose_(previous_pose);
			break;
		}
	}
	if(cb) cb(iteration_state(estimated_transformation_, error_), true);
}


void iterative_correspondences_registration_base::apply_estimated_transformation() {
	pose ps = this->get_pose_();
	ps = ps.transform( estimated_transformation_.inverse() );
	this->set_pose_(ps);
}


}