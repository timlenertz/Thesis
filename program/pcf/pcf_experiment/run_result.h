#ifndef PCFEX_RUN_RESULT_H_
#define PCFEX_RUN_RESULT_H_

#include <chrono>
#include <vector>
#include <Eigen/Geometry>
#include <memory>
#include <utility>
#include <string>

namespace pcf {

class color_image;

namespace exper {


/**
Results from one registration run.
Stored and retrieved into database by results class.
*/
class run_result {
public:
	/// State at one iteration of the registration.
	struct state {
		float error; ///< Error metric at this state, as measured by the used correspondences algorithm.
		float actual_error; ///< Error metric at this state, measured knowing the true point correspondences.
		Eigen::Affine3f transformation; ///< Transformation at this state.
		std::chrono::milliseconds time; ///< Elapsed time since start of run.
		std::unique_ptr<color_image> snapshot;
	
		state() = default;
		state(state&&);
		state& operator=(state&&);
		~state();
	};
	
	std::vector<state> evolution; ///< States of the registration.
	bool success; ///< Whether the registration was deemed successful by registration algorithm.
	bool actual_success; ///< Whether the registration was successful by looking at real point correspondences.
	Eigen::Affine3f original_transformation; ///< Initial transformation applied to loose point cloud.
	float registration_arg; ///< Argument for registration.
	float displacer_arg; ///< Argument for displacer.
	float make_fixed_arg; ///< Argument for fixed point cloud modifier.
	float make_loose_arg; ///< Argument for loose point cloud modifier.
	std::size_t loose_number_of_points;
	std::size_t fixed_number_of_points;
	
	void export_animation(const std::string& filename, const char* format) const;
};


}
}

#endif