#include <mutex>
#include <utility>
#include <functional>
#include "iterative_correspondences_registration.h"

#include <iostream>

namespace pcf {

void iterative_correspondences_registration_base::reset() {
	current_error_ = NAN;
	accumulated_transformation_.setIdentity();
}

bool iterative_correspondences_registration_base::run(const iteration_callback& cb) {
	reset();

	float previous_error = INFINITY;
	current_error_ = INFINITY;

	std::size_t iteration_count = 1;	
	while(iteration_count++ < maximal_iterations && current_error_ > minimal_error) {
		previous_error = current_error_;
		this->compute_estimated_transformation_and_error();
	
		float error_diff = previous_error - current_error_;
		if(stop_on_divergence && error_diff <= -divergence_error_threshold)
			return false;

		accumulated_transformation_ = estimated_transformation_ * accumulated_transformation_;
		if(cb) cb();
	}
	
	return (current_error_ <= minimal_error);
}


void iterative_correspondences_registration_base::step() {
	this->compute_estimated_transformation_and_error();
	accumulated_transformation_ = estimated_transformation_ * accumulated_transformation_;
}


void iterative_correspondences_registration_base::apply_loose_transformation(space_object& fx, space_object& ls) {
	ls.transform(accumulated_transformation_, fx);
	accumulated_transformation_.setIdentity();
}


std::future<bool> iterative_correspondences_registration_base::run_live(space_object& fx, space_object& ls) {
	std::packaged_task<bool()> task([&] {
		auto cb = [&] {
			apply_loose_transformation(fx, ls);
		};
		return run(cb);
	});
	auto fut = task.get_future();
	std::thread th(std::move(task));
	th.detach();
	return fut;
}

}