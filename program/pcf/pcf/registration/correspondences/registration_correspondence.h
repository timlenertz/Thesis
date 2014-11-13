#ifndef PCF_REGISTRATION_CORRESPONDENCE_H_
#define PCF_REGISTRATION_CORRESPONDENCE_H_

#include <Eigen/Eigen>

namespace pcf {


/**
Correspondence between fixed and loose point cloud.
Stored using coordinates, need not be actual points in the cloud. Base class of correspondence
structure produced by correspondence generators. Can be stored in containers.
*/
struct registration_correspondence {
	Eigen::Vector3f fixed;
	Eigen::Vector3f loose;
	float weight;

	registration_correspondence(const Eigen::Vector3f& f, const Eigen::Vector3f& l, float w) :
		fixed(f), loose(l), weight(w) { }
};



template<typename Point_fixed, typename Point_loose>
struct point_correspondence : public registration_correspondence {
	const Point_fixed& fixed_point;
	const Point_loose& loose_point;

	point_correspondence(const Point_fixed& f, const Point_loose& l, float w) :
		registration_correspondence(Eigen::Vector3f(f), Eigen::Vector3f(l), w),
		fixed_point(f), loose_point(l) { }
};


}

#endif
