#ifndef PCF_ITERATIVE_CORRESPONDENCES_REGISTRATION_H_
#define PCF_ITERATIVE_CORRESPONDENCES_REGISTRATION_H_

#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include <cmath>
#include <utility>
#include <functional>
#include <vector>
#include "transformation_estimation/svd_transformation_estimation.h"
#include "../geometry/pose.h"

namespace pcf {

/**
Abstract base class for implementation of ICP registration algorithm.
*/
class iterative_correspondences_registration_base {
protected:
	Eigen::Affine3f estimated_transformation_;
	float error_;
	
	virtual const pose& get_pose_() = 0;
	virtual void set_pose_(const pose&) = 0;

public:
	struct iteration_state {
		Eigen::Affine3f estimated_transformation;
		float error;
		
		iteration_state(const Eigen::Affine3f& t, float err) : 
			estimated_transformation(t), error(err) { }
	};

	using iteration_callback = std::function<void(const iteration_state&, bool done)>;
	
	float minimal_error = 0;
	std::size_t maximal_iterations = -1;
	bool stop_on_divergence = true;
	
	virtual ~iterative_correspondences_registration_base() { }
	
	float last_error() const { return error_; }
	const Eigen::Affine3f& last_estimated_transformation() const { return estimated_transformation_; }

	void run(const iteration_callback& = iteration_callback());
	std::vector<iteration_state> run_and_get_statistics(const iteration_callback& = iteration_callback());

	void apply_estimated_transformation();

	virtual void estimate_transformation() = 0;
};



/**
Generic implementation of ICP registration algorithm.
*/
template<
	typename Correspondences,
	typename Transformation_estimation = svd_transformation_estimation,
	typename Error_metric = typename Transformation_estimation::error_metric
>
class iterative_correspondences_registration : public iterative_correspondences_registration_base {
public:
	using correspondence_type = typename Correspondences::correspondence_type;
	using fixed_point_cloud_type = typename Correspondences::fixed_point_cloud_type;
	using loose_point_cloud_type = typename Correspondences::loose_point_cloud_type;

	Correspondences correspondences;	
	const fixed_point_cloud_type& fixed;
	loose_point_cloud_type& loose;

private:
	class receiver;
	
protected:
	const pose& get_pose_() override { return loose.relative_pose(); }
	void set_pose_(const pose& ps) override { loose.set_relative_pose(ps); }

public:

	iterative_correspondences_registration(loose_point_cloud_type& cl, const Correspondences& cor) :
		correspondences(cor),
		fixed(correspondences.fixed_point_cloud()),
		loose(cl) { }
		
	iterative_correspondences_registration(loose_point_cloud_type& cl, Correspondences&& cor) :
		correspondences(std::move(cor)),
		fixed(correspondences.fixed_point_cloud()), // not use cor since it is invalidated from move construction
		loose(cl) { }

	void estimate_transformation() override;
};



template<typename Correspondences, typename... Args>
iterative_correspondences_registration<Correspondences, Args...> make_iterative_correspondences_registration
(const typename Correspondences::fixed_point_cloud_type& cf, typename Correspondences::loose_point_cloud_type& cl, const Correspondences& cor, Args&&... args) {
	return iterative_correspondences_registration<Correspondences, Args...>(cf, cl, cor, std::forward<Args>(args)...);
}



}

#include "iterative_correspondences_registration.tcc"

#endif