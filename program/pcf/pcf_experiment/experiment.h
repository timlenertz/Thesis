#ifndef PCFEX_EXPERIMENT_H_
#define PCFEX_EXPERIMENT_H_

#include "../pcf_core/point_cloud/unorganized/unorganized_point_cloud.h"
#include "../pcf_core/point_cloud/tree/kdtree_traits.h"
#include "../pcf_core/registration/iterative_correspondences_registration.h"
#include "../pcf_core/registration/correspondences/same_point_correspondences.h"
#include "experiment_results.h"
#include <Eigen/Geometry>
#include <functional>
#include <utility>

namespace pcf {

class experiment {
public:
	using fixed_point_cloud_type = kdtree_point_cloud_full;
	using loose_point_cloud_type = unorganized_point_cloud_full;
	
	using fixed_modifier_function_type =
		void (unorganized_point_cloud_full&, float arg);
	using loose_modifier_function_type =
		void (unorganized_point_cloud_full&, float arg);
	using displacer_function_type =
		Eigen::Affine3f (float arg);
	using create_registration_function_type =
		iterative_correspondences_registration_base* (const fixed_point_cloud_type&, const loose_point_cloud_type&);

	pcf::unorganized_point_cloud_full original_point_cloud;
	
	std::function<fixed_modifier_function_type> fixed_modifier;
	std::function<loose_modifier_function_type> loose_modifier;
	std::function<displacer_function_type> displacer;
	std::function<create_registration_function_type> create_registration;
	
	unsigned fixed_modifier_runs = 1;
	unsigned loose_modifier_runs = 1;
	unsigned displacer_runs = 1;
	unsigned registration_runs = 1;
	
private:
	experiment_results results_;

	static float arg_(unsigned i, unsigned n);
	experiment_results::run run_registration_(const fixed_point_cloud_type&, const loose_point_cloud_type&) const;

public:
	template<typename Other_cloud>
	explicit experiment(Other_cloud&& pc) :
		original_point_cloud(std::forward<Other_cloud>(pc)) { }
	
	void run(bool parallel = true);
};

}

#endif
