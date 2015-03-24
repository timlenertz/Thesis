#include <mutex>
#include <utility>
#include <functional>
#include "iterative_correspondences_registration.h"

namespace pcf {

void iterative_correspondences_registration_base::reset() {
	current_error_ = NAN;
	current_loose_transformation_.setIdentity();
}

bool iterative_correspondences_registration_base::run(const iteration_callback& cb) {
	reset();

	this->compute_estimated_transformation_and_error();

	std::size_t iteration_count = 1;	
	while(iteration_count++ < maximal_iterations && current_error_ > minimal_error) {
		if(cb) cb();
			
		float previous_error = 	current_error_;
		this->compute_estimated_transformation_and_error();

		float error_diff = previous_error - current_error_;
		if(stop_on_divergence && error_diff <= -divergence_error_threshold)
			return false;
		
		current_loose_transformation_ = estimated_transformation_.inverse() * current_loose_transformation_;
	}
	
	if(cb) cb();
	
	return (current_error_ <= minimal_error);
}


void iterative_correspondences_registration_base::apply_loose_transformation(space_object& loose_point_cloud) {
	loose_point_cloud.transform(current_loose_transformation_);
	current_loose_transformation_.setIdentity();
}


std::future<bool> iterative_correspondences_registration_base::run_live(space_object& loose_point_cloud) {
	std::packaged_task<bool()> task([&] {
		auto cb = [&] {
			apply_loose_transformation(loose_point_cloud);
		};
		return run(cb);
	});
	auto fut = task.get_future();
	std::thread th(std::move(task));
	th.detach();
	return fut;
}

}