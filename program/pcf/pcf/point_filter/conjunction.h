#ifndef PCF_POINT_FILTER_CONJUNCTION_H_
#define PCF_POINT_FILTER_CONJUNCTION_H_

#include "point_filter.h"
#include <tuple>
#include <utility>
#include <type_traits>

namespace pcf {

template<typename... Filters>
class point_filter_conjunction {
private:
	std::tuple<Filters...> filters_;

	template<typename Remaining> void reset_(Remaining);
	void reset_(std::integral_constant<std::size_t, 0>);
	template<typename Point, typename Remaining> bool and_(const Point&, Remaining);
	template<typename Point> bool and_(const Point&, std::integral_constant<std::size_t, 0>);

public:
	point_filter_conjunction(Filters&&... filt) :
		filters_(std::forward<Filters&&>(filt)...) { }
	
	void reset() {
		reset_<sizeof...(Filters)>();
	}
	
	bool operator()(const point_xyz& pt) {
		return and_(pt, std::integral_constant<std::size_t, sizeof...(Filters)>());
	}
	
	bool operator()(const point_full& pt) {
		return and_(
			static_cast<const point_xyz&>(pt),
			std::integral_constant<std::size_t, sizeof...(Filters)>()
		);
	}
};



template<typename... Filters> template<typename Remaining>
void point_filter_conjunction<Filters...>::reset_(Remaining) {
	const std::size_t n = sizeof...(Filters);
	std::get<n - Remaining::value>(filters_).reset();
	reset_( std::integral_constant<std::size_t, Remaining::value - 1>() );
}

template<typename... Filters>
inline void point_filter_conjunction<Filters...>::reset_(std::integral_constant<std::size_t, 0>) { }



template<typename... Filters> template<typename Point, typename Remaining>
bool point_filter_conjunction<Filters...>::and_(const Point& pt, Remaining) {
	const std::size_t n = sizeof...(Filters);
	const auto& filter = std::get<n - Remaining::value>(filters_);
	bool accept = filter(pt);
	if(accept) return and_(pt, std::integral_constant<std::size_t, Remaining::value - 1>());
	else return false;
}

template<typename... Filters> template<typename Point>
bool point_filter_conjunction<Filters...>::and_(const Point&, std::integral_constant<std::size_t, 0>) {
	return true;
}


}

#endif
