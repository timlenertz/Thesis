#ifndef PCF_CLOSEST_POINT_CORRESPONDENCES_H_
#define PCF_CLOSEST_POINT_CORRESPONDENCES_H_

#include "point_correspondences.h"

namespace pcf {

template<typename Cloud_fixed, typename Cloud_loose>
class closest_point_correspondences : public point_correspondences<Cloud_fixed, Cloud_loose> {
	using super = point_correspondences<Cloud_fixed, Cloud_loose>;
	using typename super::fixed_point_type;
	using typename super::loose_point_type;
	using typename super::correspondence_type;

public:
	closest_point_correspondences(const Cloud_fixed& cf, Cloud_loose& cl) : point_correspondences(cf, cl) { }

	void compute();
};

}

#include "closest_point_correspondences.tcc"

#endif
