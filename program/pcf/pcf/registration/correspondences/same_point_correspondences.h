#ifndef PCF_SAME_POINT_CORRESPONDENCES_H_
#define PCF_SAME_POINT_CORRESPONDENCES_H_

#include "registration_correspondence.h"

namespace pcf {

/**
Point correspondences between points at same indexed in both clouds.
Both clouds need to have same size. Assumes one cloud is a transformation of the other.
Used for measuring registration error with known correspondences.
*/
template<typename Cloud_fixed, typename Cloud_loose>
class same_point_correspondences {
public:
	using fixed_point_cloud_type = Cloud_fixed;
	using loose_point_cloud_type = Cloud_loose;


private:
	const Cloud_fixed& fixed_;
	const Cloud_loose& loose_;

public:
	using correspondence_type = point_correspondence<
		const typename Cloud_fixed::point_type,
		const typename Cloud_loose::point_type
	>;

	same_point_correspondences(const Cloud_fixed&, const Cloud_loose&);	
	template<typename Receiver> void operator()(Receiver&);
};


}

#include "same_point_correspondences.tcc"

#endif