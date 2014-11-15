#ifndef PCF_ITERATIVE_CORRESPONDENCES_REGISTRATION_H_
#define PCF_ITERATIVE_CORRESPONDENCES_REGISTRATION_H_

#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include <cmath>
#include <utility>
#include "../util/misc.h"
#include "transformation_estimation/svd_transformation_estimation.h"
#include "error_metric/mean_square_error.h"

namespace pcf {

template<
	typename Cloud_fixed,
	typename Cloud_loose,
	typename Correspondences,
	typename Transformation_estimation = svd_transformation_estimation,
	typename Error_metric = mean_square_error
>
class iterative_correspondences_registration {
private:
	using correspondence_type = typename Correspondences::correspondence_type;

	class receiver;

	const Cloud_fixed& fixed_;
	Cloud_loose& loose_;
	Correspondences correspondences_;
	
	float error_;
	Eigen::Affine3f estimated_transformation_;
	
	float minimal_error_ = 0;
	std::size_t maximal_iterations_ = 10;
		
public:
	iterative_correspondences_registration(const Cloud_fixed& cf, Cloud_loose& cl, const Correspondences& cor) :
		fixed_(cf), loose_(cl), correspondences_(cor) { }
	
	float error() const { return error_; }

	void iteration();
	void run();
};



template<typename Cf, typename Cl, typename Cr, typename Transformation_estimation, typename Error_metric>
class iterative_correspondences_registration<Cf, Cl, Cr, Transformation_estimation, Error_metric>::receiver {
public:
	Transformation_estimation transformation_estimation;
	Error_metric error_metric;
	
	receiver& operator<<(const receiver& rec) {
		transformation_estimation << rec.transformation_estimation;
		error_metric << rec.error_metric;
		return *this;			
	}
	
	template<typename Obj>
	receiver& operator<<(const Obj& obj) {
		transformation_estimation << obj;
		error_metric << obj;
		return *this;
	}
};


}

#include "iterative_correspondences_registration.tcc"

#endif