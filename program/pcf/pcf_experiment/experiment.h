#ifndef PCFEX_EXPERIMENT_H_
#define PCFEX_EXPERIMENT_H_

#include "../pcf_core/point_cloud/unorganized/unorganized_point_cloud.h"
#include "../pcf_core/point_cloud/tree/kdtree_traits.h"
#include "../pcf_core/point_cloud/grid/grid_point_cloud.h"
#include "../pcf_core/registration/iterative_correspondences_registration.h"
#include "../pcf_core/registration/correspondences/same_point_correspondences.h"
#include "../pcf_core/geometry/pose.h"
#include "results.h"
#include "run_result.h"
#include <Eigen/Geometry>
#include <functional>
#include <utility>
#include <string>

namespace pcf {

class color_image;

namespace exper {

/**
Iterative point cloud registration algorithm experiment.
Runs an iterative point cloud registration algorithm on a set of different inputs, and stores results. Uses the same point cloud as fixed cloud for each run. The loose point cloud gets algorithmically generated by modifying the fixed, in such a way that indices of the same points don't change. So the true point correspondences are known, even if the tested registration algorithm does not use them.
*/
class experiment {
public:
	using fixed_point_cloud_type = kdtree_point_cloud_full;
	using loose_point_cloud_type = unorganized_point_cloud_full;
	using working_point_cloud_type = unorganized_point_cloud_full;
	
	using modifier_function_type =
		void (working_point_cloud_type& pc, float arg);
	using displacer_function_type =
		pose (float arg);
	using create_registration_function_type =
		iterative_correspondences_registration_base* (const fixed_point_cloud_type&, const loose_point_cloud_type&, float arg);
	using create_snapshot_function_type =
		color_image (const fixed_point_cloud_type&, const loose_point_cloud_type&, const Eigen::Affine3f& transformation);
	using run_callback_function_type =
		void (const run_result&, std::ptrdiff_t i);

	const working_point_cloud_type original_point_cloud;
	
	std::function<modifier_function_type> fixed_modifier; ///< Callback which modifies the fixed point cloud. If not set, it does not get modified.
	std::function<modifier_function_type> loose_modifier; ///< Callback which modifies the loose point cloud. If not set, it does not get modified.
	std::function<displacer_function_type> displacer; ///< Callback which returns initial transformation for loose point cloud.
	std::function<create_registration_function_type> create_registration; ///< Callback which created registration object.
	std::function<create_snapshot_function_type> create_snapshot;
	std::function<run_callback_function_type> run_callback;
	
	std::size_t additional_capacity = 0;
	unsigned fixed_modifier_runs = 1;
	unsigned loose_modifier_runs = 1;
	unsigned displacer_runs = 1;
	unsigned registration_runs = 1;
	bool run_parallel = false;
	
	float minimal_actual_error = 0.0;
	bool add_snapshots_to_results = false;
	
private:
	static float arg_(unsigned i, unsigned n);
	run_result run_registration_(const fixed_point_cloud_type& fixed, const loose_point_cloud_type& loose, float arg) const;

	float actual_error_(const Eigen::Affine3f& transformation) const;

public:
	template<typename Other_cloud>
	explicit experiment(Other_cloud&& pc) :
		original_point_cloud(std::forward<Other_cloud>(pc)) { }
	
	/// Runs the experiment and returns recorded results.
	/// If parallel, recorded times may be less meaningful. If not the correspondence finding algorithm may still be parallelized.
	results run(const std::string& db = "");
};

}
}

#endif
