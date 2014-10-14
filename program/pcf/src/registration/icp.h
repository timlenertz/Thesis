#ifndef PCF_ICP_H_
#define PCF_ICP_H_

#include "points_correspondence.h"

namespace pcf {

template<typename Cloud_fixed, typename Cloud_loose>
class icp {
public:
	using fixed_point_type = typename Cloud_fixed::point_type;
	using loose_point_type = typename Cloud_loose::point_type;
	
private:
	const Cloud_fixed& fixed_cloud_;
	Cloud_loose& loose_cloud_;
	points_correspondence<Cloud_fixed, Cloud_loose> correspondence_;
	
	void match_closest_points_();
	void apply_iteration_transformation_();
	
public:
	icp(const Cloud_fixed& pf, Cloud_loose& pl) :
		fixed_cloud_(pf), loose_cloud_(pl), correspondence_(pf, pl) { }
	
	void operator()();
};

}

#include "icp.tcc"

#endif