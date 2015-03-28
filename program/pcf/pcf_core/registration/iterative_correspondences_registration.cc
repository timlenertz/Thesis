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

		std::cout << "\n\nEstimated:\n"  << estimated_transformation_.matrix() << std::endl;
		std::cout << "Accumulated:\n"  << accumulated_transformation_.matrix() << std::endl;


		if(cb) cb();
	}
	
	return (current_error_ <= minimal_error);
}


void iterative_correspondences_registration_base::apply_loose_transformation(space_object& fx, space_object& ls, space_object& lsorig) {
	Eigen::Affine3f fx_trans = fx.relative_pose().transformation_to_world();
	Eigen::Affine3f ls_trans = ls.relative_pose().transformation_to_world();
	Eigen::Affine3f lsorig_trans = lsorig.relative_pose().transformation_to_world();
	
	Eigen::Affine3f new_ls_trans = fx_trans * accumulated_transformation_ * fx_trans.inverse() * lsorig_trans;
	
	ls.set_relative_pose(new_ls_trans);	
	//accumulated_transformation_.setIdentity();
}


std::future<bool> iterative_correspondences_registration_base::run_live(space_object& fx, space_object& ls, space_object& lsorig) {
	std::packaged_task<bool()> task([&] {
		auto cb = [&] {
			apply_loose_transformation(fx, ls, lsorig);
		};
		return run(cb);
	});
	auto fut = task.get_future();
	std::thread th(std::move(task));
	th.detach();
	return fut;
}

}