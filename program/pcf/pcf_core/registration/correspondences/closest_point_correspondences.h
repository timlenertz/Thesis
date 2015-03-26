#ifndef PCF_CLOSEST_POINT_CORRESPONDENCES_H_
#define PCF_CLOSEST_POINT_CORRESPONDENCES_H_

#include <cmath>
#include "registration_correspondence.h"
#include "../weights/equal.h"
#include "../../point_filter/accept.h"

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
	typename Selection_func = accept_point_filter,
	typename Weight_func = equal_correspondences_weights
>
class closest_point_correspondences {
public:
	using fixed_point_cloud_type = Cloud_fixed;
	using loose_point_cloud_type = Cloud_loose;
	using fixed_point_type = typename Cloud_fixed::point_type;
	using loose_point_type = typename Cloud_loose::point_type;

private:
	const Cloud_fixed& fixed_;
	const Cloud_loose& loose_;
	Selection_func selection_func_;
	Weight_func weight_func_;
	
	template<typename Receiver>
	void process_point_(const loose_point_type& transformed, const loose_point_type& real, Receiver&);

public:
	float accepting_distance = 0;
	float rejecting_distance = INFINITY;

	using correspondence_type = point_correspondence<
		const fixed_point_type,
		const loose_point_type
	>;

	closest_point_correspondences
	(const Cloud_fixed& cf, const Cloud_loose& cl, const Selection_func& sel = Selection_func(), const Weight_func& wgh = Weight_func()) :
		fixed_(cf), loose_(cl), selection_func_(sel), weight_func_(wgh) { }
	
	closest_point_correspondences(const closest_point_correspondences&) = default;
	closest_point_correspondences(closest_point_correspondences&&) = default;
	
	template<typename Receiver>
	void operator()(Receiver&, const Eigen::Affine3f& transformation = Eigen::Affine3f::Identity());
	
	const Cloud_fixed& fixed_point_cloud() const { return fixed_; }
	const Cloud_loose& loose_point_cloud() const { return loose_; }
};



template<typename Cloud_fixed, typename Cloud_loose, typename... Args>
closest_point_correspondences<Cloud_fixed, Cloud_loose, Args...> make_closest_point_correspondences
(const Cloud_fixed& cf, const Cloud_loose& cl, Args&&... args) {
	return closest_point_correspondences<Cloud_fixed, Cloud_loose, Args...>(cf, cl, std::forward<Args>(args)...);
}



template<typename Cloud_fixed, typename Cloud_loose, typename... Args>
iterative_correspondences_registration<closest_point_correspondences<Cloud_fixed, Cloud_loose, Args...>> make_iterative_closest_point_registration
(const Cloud_fixed& cf, const Cloud_loose& cl, Args&&... args) {
	auto cor = make_closest_point_correspondences(cf, cl, std::forward<Args>(args)...);
	return iterative_correspondences_registration<decltype(cor)>(cor);
}


template<typename Cloud_fixed, typename Cloud_loose, typename... Args>
iterative_correspondences_registration<closest_point_correspondences<Cloud_fixed, Cloud_loose, Args...>>* create_iterative_closest_point_registration
(const Cloud_fixed& cf, const Cloud_loose& cl, Args&&... args) {
	auto cor = make_closest_point_correspondences(cf, cl, std::forward<Args>(args)...);
	return new iterative_correspondences_registration<decltype(cor)>(cor);
}


}

#include "closest_point_correspondences.tcc"

#endif