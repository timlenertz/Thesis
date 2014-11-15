#ifndef PCF_CLOSEST_POINT_CORRESPONDENCES_H_
#define PCF_CLOSEST_POINT_CORRESPONDENCES_H_

#include "registration_correspondence.h"

namespace pcf {

/**
Point correspondences between closest points from two clouds.
Selection_func takes one point from loose cloud as argument. If true is returned, closest point to
it in fixed cloud is searched. Uses fixed cloud's find_closest_point function for this.
When corresponding point was found, calls Weight_func with two points (loose's, fixed's) as arguments.
Function must return weight for the correspondence pair. If 0.0 is returned, correspondence pair is rejected.
Both functions will be called from different threads concurrently.
*/
template<
	typename Cloud_fixed,
	typename Cloud_loose,
	typename Selection_func,
	typename Weight_func
>
class closest_point_correspondences {
private:
	const Cloud_fixed& fixed_;
	const Cloud_loose& loose_;
	Selection_func selection_func_;
	Weight_func weight_func_;

public:
	using correspondence_type = point_correspondence<
		const typename Cloud_fixed::point_type,
		const typename Cloud_loose::point_type
	>;

	closest_point_correspondences
	(const Cloud_fixed& cf, const Cloud_loose& cl, const Selection_func& sel = Selection_func(), const Weight_func& wgh = Weight_func()) :
		fixed_(cf), loose_(cl), selection_func_(sel), weight_func_(wgh) { }
	
	template<typename Receiver> void operator()(Receiver&);
};



template<typename Cloud_fixed, typename Cloud_loose, typename... Args>
closest_point_correspondences<Cloud_fixed, Cloud_loose, Args...> make_closest_point_correspondences
(const Cloud_fixed& cf, const Cloud_loose& cl, Args&&... args) {
	return closest_point_correspondences<Cloud_fixed, Cloud_loose, Args...>(cf, cl, std::forward<Args>(args)...);
}


}

#include "closest_point_correspondences.tcc"

#endif