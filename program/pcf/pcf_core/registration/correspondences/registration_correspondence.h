#ifndef PCF_REGISTRATION_CORRESPONDENCE_H_
#define PCF_REGISTRATION_CORRESPONDENCE_H_

#include <Eigen/Eigen>
#include <iostream>
#include "../../point.h"


namespace pcf {


/**
Correspondence between fixed and loose point cloud.
Stored using coordinates, need not be actual points from the cloud. Stored transformed version of loose point coordinates. Base class of correspondence structure produced by correspondence generators. Can be stored in containers.
*/
struct registration_correspondence {
	Eigen::Vector3f fixed;
	Eigen::Vector3f fixed_normal;
	Eigen::Vector3f loose;
	Eigen::Vector3f loose_normal;
	float weight;

	registration_correspondence(const Eigen::Vector3f& f, const Eigen::Vector3f& l, float w) :
		fixed(f), fixed_normal(Eigen::Vector3f::Zero()), loose(l), loose_normal(Eigen::Vector3f::Zero()), weight(w) { }

	registration_correspondence(const Eigen::Vector3f& f, const Eigen::Vector3f& fn, const Eigen::Vector3f& l, const Eigen::Vector3f& ln, float w) :
		fixed(f), fixed_normal(fn), loose(l), loose_normal(ln), weight(w) { }
};



/**
Correspondence of points between fixed and loose point cloud.
Correspondence between to actual points from the clouds. Unlike base registration_correspondence, meant to exist temporarily. Contains references to the points. Underlying registration_correspondence still stores transformed coordinates of loose point, but this object keeps reference to the actual loose point from the point cloud which may be different. The transformed loose point might be a temporary object, if for example transform_iterator is being used.
*/
template<typename Point_fixed, typename Point_loose>
struct point_correspondence : public registration_correspondence {	
	const Point_fixed& fixed_point;
	const Point_loose& loose_point;

	point_correspondence(const Point_fixed& f, const Point_fixed& transformed_l, const Point_loose& real_l, float w) :
		registration_correspondence(f, f.get_normal(), transformed_l, transformed_l.get_normal(), w),
		fixed_point(f), loose_point(real_l) { }
};

using point_correspondence_full = point_correspondence<const point_full, const point_full>;


}

#endif
