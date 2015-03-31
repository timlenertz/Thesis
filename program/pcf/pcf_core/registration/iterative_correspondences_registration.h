#ifndef PCF_ITERATIVE_CORRESPONDENCES_REGISTRATION_H_
#define PCF_ITERATIVE_CORRESPONDENCES_REGISTRATION_H_

#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include <cmath>
#include <utility>
#include <functional>
#include <vector>
#include <future>
#include "../space_object.h"
#include "transformation_estimation/svd_transformation_estimation.h"
#include "../geometry/pose.h"

namespace pcf {

/**
Abstract base class for implementation of ICP registration algorithm.
See iterative_correspondences_registration for description.
*/
class iterative_correspondences_registration_base {
protected:
	/// Current transformation of loose point cloud.
	Eigen::Affine3f accumulated_transformation_ = Eigen::Affine3f::Identity();

	/// Current estimate of transformation.
	Eigen::Affine3f estimated_transformation_;
	
	/// Previous error metric.
	/// Correspondences error metric, when accumulated_transformation_ is applied.
	float current_error_ = NAN;
	
public:
	using iteration_preprocess_callback = std::function<void()>;
	using iteration_callback = std::function<void()>;
	
	float minimal_error = 0;
	std::size_t maximal_iterations = 30;
	bool stop_on_divergence = false;
	float divergence_error_threshold = 0.0;
	iteration_preprocess_callback iteration_preprocess;
	
	virtual ~iterative_correspondences_registration_base() { }
	
	float current_error() const { return current_error_; }
	const Eigen::Affine3f& accumulated_transformation() const { return accumulated_transformation_; }
	const Eigen::Affine3f& estimated_transformation() const { return estimated_transformation_; }

	void reset();
	void iteration();
	bool run(const iteration_callback& = iteration_callback());

	void apply_loose_transformation(space_object& fx, space_object& ls);
	std::future<bool> run_live(space_object& fx, space_object& ls, const iteration_callback& = iteration_callback());

	virtual void compute_estimated_transformation_and_error() = 0;
};



/**
Generic implementation of ICP registration algorithm.
The poses of the point clouds are taken into consideration when computing correspondences or distances of points from the two clouds. Does not modify the loose points cloud's pose during the iterations. Instead stores accumulated estimated transformation in current_transformation_;
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

	void compute_estimated_transformation_and_error() override;
};

}

#include "iterative_correspondences_registration.tcc"

#endif