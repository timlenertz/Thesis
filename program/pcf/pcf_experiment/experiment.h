#ifndef PCFEX_EXPERIMENT_H_
#define PCFEX_EXPERIMENT_H_

#include "../pcf_core/point_cloud/unorganized/unorganized_point_cloud.h"
#include "../pcf_core/point_cloud/tree/kdtree_traits.h"
#include "../pcf_core/registration/iterative_correspondences_registration.h"
#include "../pcf_core/registration/correspondences/same_point_correspondences.h"
#include "results.h"
#include <Eigen/Geometry>
#include <functional>
#include <utility>

namespace pcf {
namespace exper {

/**
Iterative point cloud registration algorithm experiment.
Runs an iterative point cloud registration algorithm on a set of different inputs, and stores results. Uses the same point cloud as fixed cloud for each run. The loose point cloud gets algorithmically generated by modifying the fixed, in such a way that indices of the same points don't change. So the true point correspondences are known, even if the tested registration algorithm does not use them.

Four callbacks can be provided: Fixed modifier: applies a modification to the fixed point cloud. Loose modified: applies a modification to the loose point cloud. Displacer applies an initial transformation to the loose point cloud. These three get called a given number of times, specified using the fixed_modifier_runs etc. variables, and get passed an argument ranging from 0.0 to 1.0. Each time they receive (a copy of) the original, unmodified point cloud. 

Additionally, the create registration modifier shall return a pointer to a newly created iterative_correspondences_registration_base for the two point clouds (passed as arguments). This class takes ownership of that object (i.e. deletes it after the registration was run). Because the registration algorithm may use randomization (e.g. for selecting points), it can also be made to run multiple times with the same modifications and displacement (registration_runs).
*/
class experiment {
public:
	using fixed_point_cloud_type = kdtree_point_cloud_full;
	using loose_point_cloud_type = unorganized_point_cloud_full;
	
	using modifier_function_type =
		void (unorganized_point_cloud_full& pc, float arg);
	using displacer_function_type =
		Eigen::Affine3f (float arg);
	using create_registration_function_type =
		iterative_correspondences_registration_base* (const fixed_point_cloud_type&, const loose_point_cloud_type&, float arg);

	const pcf::unorganized_point_cloud_full original_point_cloud;
	
	std::function<modifier_function_type> fixed_modifier; ///< Callback which modifies the fixed point cloud. If not set, it does not get modified.
	std::function<modifier_function_type> loose_modifier; ///< Callback which modifies the loose point cloud. If not set, it does not get modified.
	std::function<displacer_function_type> displacer; ///< Callback which returns initial transformation for loose point cloud.
	std::function<create_registration_function_type> create_registration; ///< Callback which created registration object.
	
	std::size_t additional_capacity = 0;
	unsigned fixed_modifier_runs = 1;
	unsigned loose_modifier_runs = 1;
	unsigned displacer_runs = 1;
	unsigned registration_runs = 1;
	
private:
	static float arg_(unsigned i, unsigned n);
	results::run run_registration_(const unorganized_point_cloud_full& fixed_unorg, const fixed_point_cloud_type&, const loose_point_cloud_type&, float arg) const;

public:
	template<typename Other_cloud>
	explicit experiment(Other_cloud&& pc) :
		original_point_cloud(std::forward<Other_cloud>(pc)) { }
	
	/// Runs the experiment and returns recorded results.
	/// If parallel, recorded times may be less meaningful. If not the correspondence finding algorithm may still be parallelized.
	results run(bool parallel = false);
};

}
}

#endif
