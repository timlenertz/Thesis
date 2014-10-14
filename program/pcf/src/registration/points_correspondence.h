#ifndef PCF_POINTS_CORRESPONDENCE_H_
#define PCF_POINTS_CORRESPONDENCE_H_

#include <vector>
#include <utility>
#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include "../point.h"

namespace pcf {

template<typename Cloud_fixed, typename Cloud_loose>
class points_correspondence {
public:
	using fixed_point_type = typename Cloud_fixed::point_type;
	using loose_point_type = typename Cloud_loose::point_type;

	struct correspondence {
		const fixed_point_type& fixed;
		const loose_point_type& loose;
		float weight = 1.0;
		
		correspondence(const fixed_point_type& f, const loose_point_type& l) : fixed(f), loose(l) { }
	};

private:	
	std::vector<correspondence> correspondences_;

	void centers_of_mass_(Eigen::Vector4f& fixed, Eigen::Vector4f& loose) const;
	Eigen::Matrix3f correlation_matrix_(const Eigen::Vector4f& fixed_center, const Eigen::Vector4f& loose_center) const;

public:
	void erase() {
		correspondences_.clear();
	}
		
	correspondence& add(const fixed_point_type& pf, const loose_point_type& pl) {
		if(pf.valid() && pl.valid()) {
			correspondences_.emplace_back(pf, pl);
		}
		return correspondences_.back();
	}
	
	std::size_t size() const { return correspondences_.size(); }	

	template<typename Distance_func> float error(Distance_func dist) const;
		
	Eigen::Affine3f estimate_transformation_svd() const;
};

}

#include "points_correspondence.tcc"

#endif