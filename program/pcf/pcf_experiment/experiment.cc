#include "experiment.h"
#include <stdexcept>
#include <chrono>
#include <memory>

#include <iostream>

namespace pcf {


experiment_results::run experiment::run_registration_(const fixed_point_cloud_type& fixed, const loose_point_cloud_type& loose) const {
	using reg_t = iterative_correspondences_registration_base;
	using clock_t = std::chrono::system_clock;
	using same_cor_t = same_point_correspondences<fixed_point_cloud_type, loose_point_cloud_type>;

	experiment_results::run res_run;
	std::unique_ptr<reg_t> reg(create_registration(fixed, loose));

	clock_t::time_point now = clock_t::now();
/*
	res_run.success = reg->run([&](const Eigen::Affine3f& estimated_transformation, float error) {
		same_cor_t same_cor(fixed, loose);
		mean_square_error actual_error_metric;
		same_cor(actual_error_metric, estimated_transformation);

		clock_t::time_point before = now;
		now = clock_t::now();
		
		experiment_results::state res_state;
		res_state.error = error;
		res_state.actual_error = actual_error_metric();
		res_state.transformation = estimated_transformation;
		res_state.time = std::chrono::duration_cast<std::chrono::milliseconds>(now - before);
		res_run.evolution.push_back(res_state);
	});
*/

	std::cout << "ran" << std::endl;

	return res_run;
}


float experiment::arg_(unsigned i, unsigned n) {
	if(n <= 1) return 0.0;
	else return float(i) / (n - 1);
}


void experiment::run(bool par) {
	results_.clear();

	#pragma omp parallel if(false)
	{		
	for(unsigned i = 0; i < fixed_modifier_runs; ++i) { std::cout << "i=" << i << std::endl;
		unorganized_point_cloud_full fixed_unorg = original_point_cloud;
		fixed_unorg.set_relative_pose(pose());
		float fixed_modifier_arg = arg_(i, fixed_modifier_runs);
		if(fixed_modifier) fixed_modifier(fixed_unorg, fixed_modifier_arg);
		kdtree_point_cloud_full fixed(std::move(fixed_unorg));
		
		for(unsigned j = 0; j < loose_modifier_runs; ++j) { std::cout << "j=" << j << std::endl;
			unorganized_point_cloud_full loose = original_point_cloud;
			loose.set_relative_pose(pose());
			float loose_modifier_arg = arg_(j, loose_modifier_runs);
			if(loose_modifier) loose_modifier(loose, loose_modifier_arg);
			
			for(unsigned k = 0; k < displacer_runs; ++k) { std::cout << "k=" << k << std::endl;
				Eigen::Affine3f original_transformation = displacer(arg_(k, displacer_runs));
				if(displacer) original_transformation = displacer(arg_(k, displacer_runs));
				else original_transformation = Eigen::Affine3f::Identity();
				loose.set_relative_pose(pose(original_transformation));
				
				for(unsigned l = 0; l < registration_runs; ++l) { std::cout << "l=" << l << std::endl;
					std::cout << "Run..." << std::endl;
				
					if(false){
					std::cout << "ready" << std::endl;

						experiment_results::run run_res = run_registration_(fixed, loose);
						run_res.original_transformation = original_transformation;
						run_res.fixed_modifier_arg = fixed_modifier_arg;
						run_res.loose_modifier_arg = loose_modifier_arg;
						
						#pragma omp critical
						{ results_.add(run_res); }
					}
					run_registration_(fixed, loose);
					std::cout << "end l" << std::endl;
				}
				
				loose.set_relative_pose(pose());
				std::cout << "end k" << std::endl;
			}
			std::cout << "end j" << std::endl;
		}
		std::cout << "end i" << std::endl;
	}
	}
}

}
