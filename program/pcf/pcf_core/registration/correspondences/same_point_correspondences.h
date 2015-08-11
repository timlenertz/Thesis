#ifndef PCF_SAME_POINT_CORRESPONDENCES_H_
#define PCF_SAME_POINT_CORRESPONDENCES_H_

#include "registration_correspondence.h"

namespace pcf {

/**
Point correspondences between points at same indexed in both clouds.
Assumes one cloud is a transformation of the other. Used for measuring registration error with known correspondences.
*/
template<typename Cloud_fixed, typename Cloud_loose>
class same_point_correspondences {
public:
	using fixed_point_cloud_type = Cloud_fixed;
	using loose_point_cloud_type = Cloud_loose;
	using fixed_point_type = typename Cloud_fixed::point_type;
	using loose_point_type = typename Cloud_loose::point_type;

private:
	const Cloud_fixed& fixed_;
	const Cloud_loose& loose_;

public:
	using correspondence_type = point_correspondence<
		const fixed_point_type,
		const loose_point_type
	>;
	
	enum {
		same_index,
		index_attribute,
		known_transformation
	} mode = same_index;
	Eigen::Affine3f real_loose_transformation = Eigen::Affine3f::Identity();
	
	same_point_correspondences(const Cloud_fixed&, const Cloud_loose&);	

	template<typename Receiver>
	void operator()(Receiver&, const Eigen::Affine3f& = Eigen::Affine3f::Identity()) const;
	
	const Cloud_fixed& fixed_point_cloud() const { return fixed_; }
	const Cloud_loose& loose_point_cloud() const { return loose_; }
};

template<typename Cloud_fixed, typename Cloud_loose>
same_point_correspondences<Cloud_fixed, Cloud_loose> make_same_point_correspondences
(const Cloud_fixed& cf, const Cloud_loose& cl) {
	return same_point_correspondences<Cloud_fixed, Cloud_loose>(cf, cl);
};


}

#include "same_point_correspondences.tcc"

#endif