#ifndef PCFEX_EXPERIMENT_RESULTS_H_
#define PCFEX_EXPERIMENT_RESULTS_H_

#include <chrono>
#include <vector>

namespace pcf {

class experiment_results {
public:
	struct state {
		float error;
		float actual_error;
		Eigen::Affine3f transformation;
		std::chrono::milliseconds time;
	};
	
	struct run {
		std::vector<state> evolution;
		bool success;
		Eigen::Affine3f original_transformation;
		float fixed_modifier_arg;
		float loose_modifier_arg;
		float displacement_arg;
	};
};

}

#endif
