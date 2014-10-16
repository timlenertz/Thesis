#ifndef PCF_POINT_CORRESPONCENCE_H_
#define PCF_POINT_CORRESPONCENCE_H_

#include <cassert>

namespace pcf {

template<typename Point_fixed, typename Point_loose>
class point_correspondence {
private:
	const Point_fixed* fixed_;
	const Point_loose* loose_;
	
public:
	point_correspondence(const point_correspondence&) = default;
	point_correspondence(const Point_fixed& pf, const Point_loose& pl) :
	fixed_(&pf), loose_(&pl) {
		assert(pf.valid()); assert(pl.valid());
	}
	
	const Point_fixed& fixed_point() const { return *fixed_; }
	const Point_loose& loose_point() const { return *loose_; }
};

}

#endif