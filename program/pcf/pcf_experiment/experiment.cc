#include "experiment.h"
#include <stdexcept>
#include <chrono>
#include <memory>
#include <utility>
#include "../pcf_core/image/color_image.h"

#include <iostream>

namespace pcf {
namespace exper {

run_result experiment::run_registration_(const fixed_point_cloud_type& fixed, const loose_point_cloud_type& loose, float arg) const {
	using reg_t = iterative_correspondences_registration_base;
	using clock_t = std::chrono::system_clock;

	same_point_correspondences<working_point_cloud_type, working_point_cloud_type> same_cor(original_point_cloud, original_point_cloud);

	run_result res_run;
	std::unique_ptr<reg_t> reg(create_registration(fixed, loose, arg));

	clock_t::time_point start_time = clock_t::now();

	res_run.success = reg->run([&]() {
		Eigen::Affine3f absolute_transformation = reg->current_loose_transformation() * loose.transformation_from(fixed);
	
		mean_square_error actual_error_metric;
		same_cor(actual_error_metric, absolute_transformation);

		run_result::state res_state;
		res_state.error = reg->current_error();
		res_state.actual_error = actual_error_metric();
		res_state.transformation = reg->current_loose_transformation();
		res_state.time = std::chrono::duration_cast<std::chrono::milliseconds>(clock_t::now() - start_time);
				
		if(create_snapshot) {
			color_image img = create_snapshot(fixed, loose, reg->current_loose_transformation());
			res_state.snapshot.reset(new color_image(std::move(img)));
		}
		
		res_run.evolution.push_back(std::move(res_state));
	});
	
	res_run.actual_success = (res_run.evolution.back().actual_error <= minimal_actual_error);

	return res_run;
}


float experiment::arg_(unsigned i, unsigned n) {
	if(n <= 1) return 0.0;
	else return float(i) / (n - 1);
}


results experiment::run(const std::string& db) {
	unsigned total = fixed_modifier_runs * loose_modifier_runs * displacer_runs * registration_runs;
	unsigned counter = 0;

	results res(db);
	res.clear();
	
	std::size_t cap = original_point_cloud.size() + additional_capacity;

	for(unsigned i = 0; i < fixed_modifier_runs; ++i) {
		working_point_cloud_type fixed_unorg(original_point_cloud, cap);
		fixed_unorg.set_relative_pose(pose());
		float fixed_modifier_arg = arg_(i, fixed_modifier_runs);
		if(fixed_modifier) fixed_modifier(fixed_unorg, fixed_modifier_arg);
		fixed_point_cloud_type fixed(std::move(fixed_unorg));
		
		for(unsigned j = 0; j < loose_modifier_runs; ++j) {
			working_point_cloud_type loose(original_point_cloud, cap);
			loose.set_relative_pose(pose());
			float loose_modifier_arg = arg_(j, loose_modifier_runs);
			if(loose_modifier) loose_modifier(loose, loose_modifier_arg);
			
			#pragma omp parallel for if(displacer_runs > 1 && run_parallel)
			for(unsigned k = 0; k < displacer_runs; ++k) {
				pose transformation;
				float displacer_arg = arg_(k, displacer_runs);
				if(displacer) transformation = displacer(displacer_arg);
				
				#pragma omp parallel for if(registration_runs > 1 && run_parallel)
				for(unsigned l = 0; l < registration_runs; ++l) {
					float registration_arg = arg_(l, registration_runs);
					run_result run_res;
					
					if(run_parallel) {
						// Currently need to create copy of loose for each thread, because it uses the loose's pose
						working_point_cloud_type displaced_loose = loose;
						displaced_loose.set_relative_pose(transformation);
						run_res = run_registration_(fixed, displaced_loose, registration_arg);						
					} else {
						loose.set_relative_pose(transformation);
						run_res = run_registration_(fixed, loose, registration_arg);	
					}
					
					run_res.original_transformation = transformation.transformation_from_world();
					run_res.registration_arg = registration_arg;
					run_res.displacer_arg = displacer_arg;
					run_res.fixed_modifier_arg = fixed_modifier_arg;
					run_res.loose_modifier_arg = loose_modifier_arg;
					
					if(run_callback) run_callback(run_res);
					
					#pragma omp critical
					{
						res.add(run_res);
						++counter;
						float ferr = run_res.evolution.back().error;
						std::cout << counter << " from " << total << " (suc: " << run_res.success << ", states: " << run_res.evolution.size() << ", err: " << ferr << ")" << std::endl;
					}
				}
			}
		}
	}
	
	return res;
}

}
}
