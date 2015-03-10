#include "experiment.h"
#include <stdexcept>
#include <chrono>
#include <memory>

#include <iostream>

namespace pcf {
namespace exper {

results::run experiment::run_registration_(const fixed_point_cloud_type& fixed, const loose_point_cloud_type& loose) const {
	using reg_t = iterative_correspondences_registration_base;
	using clock_t = std::chrono::system_clock;
	using same_cor_t = same_point_correspondences<fixed_point_cloud_type, loose_point_cloud_type>;

	results::run res_run;
	std::unique_ptr<reg_t> reg(create_registration(fixed, loose));

	clock_t::time_point now = clock_t::now();

	res_run.success = reg->run([&](const Eigen::Affine3f& estimated_transformation, float error) {
		same_cor_t same_cor(fixed, loose);
		mean_square_error actual_error_metric;
		same_cor(actual_error_metric, estimated_transformation);

		clock_t::time_point before = now;
		now = clock_t::now();
		
		results::state res_state;
		res_state.error = error;
		res_state.actual_error = actual_error_metric();
		res_state.transformation = estimated_transformation;
		res_state.time = std::chrono::duration_cast<std::chrono::milliseconds>(now - before);
		res_run.evolution.push_back(res_state);
	});

	return res_run;
}


float experiment::arg_(unsigned i, unsigned n) {
	if(n <= 1) return 0.0;
	else return float(i) / (n - 1);
}


results experiment::run(bool par) {
	unsigned total = fixed_modifier_runs * loose_modifier_runs * displacer_runs * registration_runs;
	unsigned counter = 0;

	results res;

	for(unsigned i = 0; i < fixed_modifier_runs; ++i) {
		unorganized_point_cloud_full fixed_unorg = original_point_cloud;
		fixed_unorg.set_relative_pose(pose());
		float fixed_modifier_arg = arg_(i, fixed_modifier_runs);
		if(fixed_modifier) fixed_modifier(fixed_unorg, fixed_modifier_arg);
		kdtree_point_cloud_full fixed(std::move(fixed_unorg));
		
		for(unsigned j = 0; j < loose_modifier_runs; ++j) {
			unorganized_point_cloud_full loose = original_point_cloud;
			loose.set_relative_pose(pose());
			float loose_modifier_arg = arg_(j, loose_modifier_runs);
			if(loose_modifier) loose_modifier(loose, loose_modifier_arg);
			
			#pragma omp parallel for if(displacer_runs > 1 && par)
			for(unsigned k = 0; k < displacer_runs; ++k) {
				Eigen::Affine3f transformation = Eigen::Affine3f::Identity();
				float displacer_arg = arg_(k, displacer_runs);
				if(displacer) transformation = displacer(displacer_arg);
				
				#pragma omp parallel for if(registration_runs > 1 && par)
				for(unsigned l = 0; l < registration_runs; ++l) {	
					results::run run_res;
					
					if(par) {
						// Currently need to create copy of loose for each thread, because it uses the loose's pose
						unorganized_point_cloud_full displaced_loose = loose;
						displaced_loose.set_relative_pose(pose(transformation));
						run_res = run_registration_(fixed, displaced_loose);						
					} else {
						loose.set_relative_pose(pose(transformation));
						run_res = run_registration_(fixed, loose);	
					}
					
					run_res.original_transformation = transformation;
					run_res.displacer_arg = displacer_arg;
					run_res.fixed_modifier_arg = fixed_modifier_arg;
					run_res.loose_modifier_arg = loose_modifier_arg;
					
					#pragma omp critical
					{
						res.add(run_res);
						++counter;
						std::cout << counter << " from " << total << " (states: " << run_res.evolution.size() << ")" << std::endl;
					}
				}
			}
		}
	}
	
	return res;
}

}
}
