#ifndef PCF_REGISTRATION_CORRESPONDENCE_H_
#define PCF_REGISTRATION_CORRESPONDENCE_H_

#include <Eigen/Eigen>

namespace pcf {


/**
Correspondence between fixed and loose point cloud.
Stored using coordinates, need not be actual points from the cloud. Stored transformed version of loose point coordinates. Base class of correspondence structure produced by correspondence generators. Can be stored in containers.
*/
struct registration_correspondence {
	Eigen::Vector3f fixed;
	Eigen::Vector3f loose;
	float weight;

	registration_correspondence(const Eigen::Vector3f& f, const Eigen::Vector3f& l, float w) :
		fixed(f), loose(l), weight(w) { }
};



/**
Correspondence of points between fixed and loose point cloud.
Correspondence between to actual points from the clouds. Unlike base registration_correspondence, meant to exist temporarily. Contains references to the points. Underlying registration_correspondence still stored transformed coordinates of loose point, but this object keeps reference to the actual loose point from the point cloud which may be different. The transformed loose point might be a temporary object, if for example transform_iterator is being used.
*/
template<typename Point_fixed, typename Point_loose>
struct point_correspondence : public registration_correspondence {
	const Point_fixed& fixed_point;
	const Point_loose& loose_point;

	point_correspondence(const Point_fixed& f, const Eigen::Vector3f& transformed_l, const Point_loose& real_l, float w) :
		registration_correspondence(Eigen::Vector3f(f), transformed_l, w),
		fixed_point(f), loose_point(real_l) { }
};


}

#endif
