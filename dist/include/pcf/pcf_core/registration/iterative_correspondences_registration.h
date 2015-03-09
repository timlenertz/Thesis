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
	Eigen::Affine3f current_transformation_ = Eigen::Affine3f::Identity();
	float error_;
	
public:
	using iteration_callback = std::function<void(const Eigen::Affine3f& estimated_transformation, float error)>;
	
	float minimal_error = 0;
	std::size_t maximal_iterations = -1;
	bool stop_on_divergence = true;
	
	virtual ~iterative_correspondences_registration_base() { }
	
	float last_error() const { return error_; }
	const Eigen::Affine3f& last_estimated_transformation() const { return current_transformation_; }

	bool run(const iteration_callback& = iteration_callback());

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
	const loose_point_cloud_type& loose;

private:
	class receiver;

public:
	explicit iterative_correspondences_registration(const Correspondences& cor) :
		correspondences(cor),
		fixed(correspondences.fixed_point_cloud()),
		loose(correspondences.loose_point_cloud()) { }
		
	explicit iterative_correspondences_registration(Correspondences&& cor) :
		correspondences(std::move(cor)),
		fixed(correspondences.fixed_point_cloud()), // not use cor since it is invalidated from move construction
		loose(correspondences.loose_point_cloud()) { }

	void estimate_transformation() override;
};

}

#include "iterative_correspondences_registration.tcc"

#endif