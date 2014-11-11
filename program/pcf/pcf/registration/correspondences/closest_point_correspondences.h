#ifndef PCF_CLOSEST_POINT_CORRESPONDENCES_H_
#define PCF_CLOSEST_POINT_CORRESPONDENCES_H_

#include "point_correspondences.h"

namespace pcf {

/**
Point correspondences between closest points from two clouds.

*/
template<typename Cloud_fixed, typename Cloud_loose, typename Selection_func, typename Weight_func>
class closest_point_correspondences : public point_correspondences<Cloud_fixed, Cloud_loose> {
	using super = point_correspondences<Cloud_fixed, Cloud_loose>;
	using typename super::correspondence;

private:
	const Cloud_fixed& fixed_;
	const Cloud_loose& loose_;
	
	Selection_func selection_func_;
	Weight_func weight_func_;

public:
	closest_point_correspondences(const Cloud_fixed& cf, const Cloud_loose& cl, const Selection_func& sel, const Weight_func& wgh) :
		fixed_(cf), loose_(cl), selection_func_(sel), weight_func_(wgh) { }
	
	void operator() ();
};


template<typename Cloud_fixed, typename Cloud_loose, typename Selection_func, typename Weight_func>
closest_point_correspondences<Cloud_fixed, Cloud_loose, Selection_func, Weight_func>
make_closest_point_correspondences(const Cloud_fixed& cf, const Cloud_loose& cl, const Selection_func& sel, const Weight_func& wgh) {
	return closest_point_correspondences<Cloud_fixed, Cloud_loose, Selection_func, Weight_func>(cf, cl, sel, wgh);
}


}

#include "closest_point_correspondences.tcc"

#endif