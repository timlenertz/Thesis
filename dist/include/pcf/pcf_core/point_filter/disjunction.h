#ifndef PCF_POINT_FILTER_DISJUNCTION_H_
#define PCF_POINT_FILTER_DISJUNCTION_H_

#include "point_filter.h"
#include <tuple>
#include <utility>
#include <type_traits>

namespace pcf {

/**
Accept point iff filter 1 OR filter 2 accept it.
Inclusive disjunction (can be both).
*/
template<typename Filter1, typename Filter2>
class point_filter_disjunction {
private:
	Filter1 filter1_;
	Filter2 filter2_;

public:
	point_filter_disjunction(Filter1&& filt1, Filter2&& filt2) :
		filter1_(std::forward<Filter1>(filt1)),
		filter2_(std::forward<Filter2>(filt2)) { }
	
	void reset() {
		filter1_.reset();
		filter2_.reset();
	}
	
	bool operator()(const point_xyz& pt) {
		return filter1_(pt) || filter2_(pt);
	}
	
	bool operator()(const point_full& pt) {
		return filter1_(pt) || filter2_(pt);
	}
};



template<
	typename Filter1, typename Filter2,
	typename Filter1_dec = typename std::decay<Filter1>::type, typename Filter2_dec = typename std::decay<Filter2>::type,
	typename = typename std::enable_if<std::is_base_of<point_filter, Filter1_dec>::value>::type
>
point_filter_disjunction<Filter1_dec, Filter2_dec> operator|| (Filter1&& filt1, Filter2&& filt2) {
	return point_filter_disjunction<Filter1_dec, Filter2_dec>(std::forward<Filter1>(filt1), std::forward<Filter2>(filt2));
}



}

#endif
