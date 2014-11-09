#ifndef PCF_ITERATIVE_CORRESPONDENCES_REGISTRATION_H_
#define PCF_ITERATIVE_CORRESPONDENCES_REGISTRATION_H_

#include <Eigen/Geometry>
#include "../util/misc.h"

namespace pcf {

template<typename Cloud_fixed, typename Cloud_loose, typename Correspondences_func, typename Transformation_estimation_func>
class iterative_correspondences_registration {
private:
	const Cloud_fixed& fixed_;
	Cloud_loose& loose_;
	Correspondences_func correspondences_;
	Transformation_estimation_func transformation_estimation_;
	
	Eigen::Affine3f transformation_;
	
public:
	iterative_correspondences_registration(const Cloud_fixed&, Cloud_loose&);
	
	
};

}

#endif