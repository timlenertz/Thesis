#ifndef PCF_POINT_CORRESPONDENCES_H_
#define PCF_POINT_CORRESPONDENCES_H_

#include <vector>
#include <utility>
#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include "point_correspondence.h"
#include "../point.h"

namespace pcf {

template<typename Cloud_fixed, typename Cloud_loose>
class point_correspondences {
public:
	using fixed_point_type = typename Cloud_fixed::point_type;
	using loose_point_type = typename Cloud_loose::point_type;
	using correspondence_type = point_correspondence<fixed_point_type, loose_point_type>;

protected:	
	const Cloud_fixed& fixed_cloud_;
	Cloud_loose& loose_cloud_;
	std::vector<correspondence_type> cors_;

	void centers_of_mass_(Eigen::Vector4f& fixed, Eigen::Vector4f& loose) const;
	Eigen::Matrix3f correlation_matrix_(const Eigen::Vector4f& fixed_center, const Eigen::Vector4f& loose_center) const;

public:
	point_correspondences(const Cloud_fixed&, Cloud_loose&);
	
	float mean_squared_error() const { return error(euclidian_distance_sq) / cors_.size(); }

	template<typename Distance_func> float error(Distance_func dist) const;
	
	Eigen::Affine3f estimate_transformation_svd() const;
};


}

#include "point_correspondences.tcc"

#endif