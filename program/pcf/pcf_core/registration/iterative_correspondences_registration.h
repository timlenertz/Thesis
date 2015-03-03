#ifndef PCF_ITERATIVE_CORRESPONDENCES_REGISTRATION_H_
#define PCF_ITERATIVE_CORRESPONDENCES_REGISTRATION_H_

#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include <cmath>
#include <utility>
#include <functional>
#include <vector>
#include "transformation_estimation/svd_transformation_estimation.h"
#include "error_metric/mean_square_error.h"

namespace pcf {

/**
Generic implementation of ICP registration algorithm.
*/
template<
	typename Correspondences,
	typename Transformation_estimation = svd_transformation_estimation,
	typename Error_metric = typename Transformation_estimation::error_metric
>
class iterative_correspondences_registration {
public:
	using correspondence_type = typename Correspondences::correspondence_type;
	using fixed_point_cloud_type = typename Correspondences::fixed_point_cloud_type;
	using loose_point_cloud_type = typename Correspondences::loose_point_cloud_type;

	struct iteration_state {
		Eigen::Affine3f estimated_transformation;
		float error;
		
		iteration_state(const Eigen::Affine3f& t, float err) : 
			estimated_transformation(t), error(err) { }
	};

private:
	class receiver;
	
	Eigen::Affine3f estimated_transformation_;
	float error_;
			
public:
	const fixed_point_cloud_type& fixed;
	loose_point_cloud_type& loose;
	Correspondences correspondences;	

	float minimal_error = 0;
	std::size_t maximal_iterations = -1;
	bool stop_on_divergence = true;

	using iteration_callback = std::function<void(const Eigen::Affine3f& est_t, float err, bool done)>;

	iterative_correspondences_registration(const fixed_point_cloud_type& cf, loose_point_cloud_type& cl, const Correspondences& cor) :
		fixed(cf), loose(cl), correspondences(cor) { }
	
	float last_error() const { return error_; }
	const Eigen::Affine3f& last_estimated_transformation() const { return estimated_transformation_; }

	void estimate_transformation();
	void apply_estimated_transformation();
	
	std::vector<iteration_state> run(const iteration_callback& = iteration_callback());
};


template<typename Correspondences, typename... Args>
iterative_correspondences_registration<Correspondences, Args...> make_iterative_correspondences_registration
(const typename Correspondences::fixed_point_cloud_type& cf, typename Correspondences::loose_point_cloud_type& cl, const Correspondences& cor, Args&&... args) {
	return iterative_correspondences_registration<Correspondences, Args...>(cf, cl, cor, std::forward<Args>(args)...);
}



}

#include "iterative_correspondences_registration.tcc"

#endif