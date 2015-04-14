#ifndef PCF_POINT_FILTER_NEGATION_H_
#define PCF_POINT_FILTER_NEGATION_H_

#include "point_filter.h"
#include <utility>
#include <type_traits>

namespace pcf {

/**
Accept point iff given filter rejects it.
*/
template<typename Filter>
class point_filter_negation : public point_filter {
private:
	Filter filter_;

public:
	template<typename... Args>
	explicit point_filter_negation(Args&&... args) :
		filter_(std::forward<Args>(args)...) { }
	
	bool operator()(const point_xyz& pt) const {
		return ! filter_(pt);
	}
	
	bool operator()(const point_full& pt) const {
		return ! filter_(pt);
	}
};

template<
	typename Filter,
	typename Filter_dec = typename std::decay<Filter>::type,
	typename = typename std::enable_if<std::is_base_of<point_filter, Filter_dec>::value>::type
>
point_filter_negation<Filter_dec> operator! (Filter&& filt) {
	return point_filter_negation<Filter_dec>(filt);
}

}

#endif
