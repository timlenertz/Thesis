#ifndef PCF_ITERATIVE_CORRESPONDENCES_REGISTRATION_H_
#define PCF_ITERATIVE_CORRESPONDENCES_REGISTRATION_H_

#include <Eigen/Geometry>
#include <cmath>
#include <utility>
#include "../util/misc.h"
#include "transformation_estimation/svd_transformation_estimation.h"

namespace pcf {

template<
	typename Cloud_fixed,
	typename Cloud_loose,
	typename Correspondences,
	typename Transformation_estimation = svd_transformation_estimation<Correspondences>
>
class iterative_correspondences_registration {
private:
	const Cloud_fixed& fixed_;
	Cloud_loose& loose_;
	Correspondences correspondences_;
	Transformation_estimation transformation_estimation_;
	
	float error_;
	Eigen::Affine3f transformation_;
	
	float minimal_error_ = 10;
	std::size_t maximal_iterations_ = 10;
	
	void initialize_();
	
public:
	iterative_correspondences_registration(const Cloud_fixed& cf, Cloud_loose& cl, const Correspondences& cor) :
		fixed_(cf), loose_(cl), correspondences_(cor), transformation_estimation_(cor) { initialize_(); }
	
	float error() const { return error_; }
	const Eigen::Affine3f& transformation() const { return transformation_; }
	
	void iteration();
	void run();
};


template<typename... Args>
iterative_correspondences_registration<Args...> make_iterative_correspondences_registration(Args&&... args) {
	return iterative_correspondences_registration<Args...>(std::forward<Args>(args)...);
}

}

#include "iterative_correspondences_registration.tcc"

#endif