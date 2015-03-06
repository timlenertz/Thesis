#include "experiment.h"
#include <stdexcept>
#include <chrono>
#include <memory>

namespace pcf {

experiment_results::run experiment::run_registration_(const fixed_point_cloud_type& fixed, const loose_point_cloud_type& loose) const {
	using reg_t = iterative_correspondences_registration_base;
	using clock_t = std::chrono::system_clock;

	experiment_results::run res_run;
	std::unique_ptr<reg_t> reg(create_registration(fixed, loose));

	clock_t::time_point now = clock_t::time_point::now();

	reg->run([&](const reg_t::iteration_state& state, bool done) {
		same_point_correspondences same_cor(fixed, loose);
		mean_square_error actual_error;
		same_cor(err, state.estimated_transformation);
		
		clock_t::time_point before = now;
		now = clock_t::time_point
		
		experiment_results::state res_state;
		res_state.error = state.error;
		res_state.actual_error = actual_error();
		res_state.transformation = state.estimated_transformation;
		res_state.time = std::chrono::milliseconds(now - before);
		res_run.evolution.push_back(state);
	});
}


float experiment::arg_(unsigned i, unsigned n) {
	if(n <= 1) return 0.0;
	else return float(i) / (n - 1);
}


void experiment::run(bool par) {
	#pragma omp parallel if(par)
	{		
	for(unsigned i = 0; i < fixed_modifier_runs; ++i) {
		unorganized_point_cloud_full fixed_unorg = original_point_cloud;
		fixed_unorg.set_relative_pose(pose());
		if(fixed_modifier)
			fixed_modifier(fixed_unorg, arg_(i, fixed_modifier_runs));
		kdtree_point_cloud_full fixed(std::move(fixed_unorg));
		
		for(unsigned j = 0; j < loose_modifier_runs; ++j) {
			unorganized_point_cloud_full loose = original_point_cloud;
			loose.set_relative_pose(pose());
			if(loose_modifier)
				loose_modifier(loose, arg_(j, loose_modifier_runs));
			
			for(unsigned k = 0; k < displacer_runs; ++k) {
				if(displacer)
					displacer(loose, arg_(k, displacer_runs));
				
				for(unsigned l = 0; l < registration_runs; ++l) {
					#pragma omp task
					{
						experiment_results::run run_res = run_registration_(fixed, loose);
						
					}
				}
				
				loose.set_relative_pose(pose());
			}
		}
	}
	}
}

}
