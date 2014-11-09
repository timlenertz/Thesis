#ifndef PCF_CLOSEST_POINT_CORRESPONDENCES_H_
#define PCF_CLOSEST_POINT_CORRESPONDENCES_H_

#include "point_correspondence.h"

namespace pcf {

/**
Point correspondences between closest points from two clouds.

*/
template<typename Cloud_fixed, typename Cloud_loose, typename Selection_func, typename Modifier_func>
class closest_point_correspondences {
private:
	const Cloud_fixed& fixed_;
	const Cloud_loose& loose_;
	
	Selection_func selection_func_;
	Modifier_func modifier_func_;

public:
	using correspondences_type =
		point_correspondences<const typename Cloud_fixed::point_type, const typename Cloud_loose::point_type>;

	closest_point_correspondences(const Cloud_fixed&, const Cloud_loose&, const Selection_func&, const Modifier_func&);
	
	correspondences_type operator() ();
};

}

#include "closest_point_correspondences.tcc"

#endif