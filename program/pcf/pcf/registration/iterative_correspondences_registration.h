#ifndef PCF_ITERATIVE_CORRESPONDENCES_REGISTRATION_H_
#define PCF_ITERATIVE_CORRESPONDENCES_REGISTRATION_H_

#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include <cmath>
#include <utility>
#include "transformation_estimation/svd_transformation_estimation.h"
#include "error_metric/mean_square_error.h"

namespace pcf {

/**
Generic implementation of ICP registration algorithm.
*/
template<
	typename Correspondences
	typename Transformation_estimation = svd_transformation_estimation,
	typename Error_metric = typename Transformation_estimation::error_metric
>
class iterative_correspondences_registration {
public:
	using correspondence_type = typename Correspondences::correspondence_type;
	using fixed_point_cloud_type = typename Correspondences::fixed_point_cloud_type;
	using loose_point_cloud_type = typename Correspondences::loose_point_cloud_type;

private:
	class receiver;

	const fixed_point_cloud_type& fixed_;
	loose_point_cloud_type& loose_;
	Correspondences correspondences_;
	
	float error_;
	Eigen::Affine3f estimated_transformation_;
			
public:
	float minimal_error = 0;
	std::size_t maximal_iterations = -1;

public:
	iterative_correspondences_registration(const fixed_point_cloud_type& cf, loose_point_cloud_type& cl, const Correspondences& cor) :
		fixed_(cf), loose_(cl), correspondences_(cor) { }
	
	float error() const { return error_; }

	void iteration();
	void run();
};


}

#include "iterative_correspondences_registration.tcc"

#endif