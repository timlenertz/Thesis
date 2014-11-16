#include <cmath>

namespace pcf {

template<typename Point, typename Iterator>
Iterator find_closest_point(const Point& ref, Iterator begin, Iterator end) {
	float min_distance = INFINITY;
	Iterator closest_point = end;
	
	for(Iterator it = begin; it != end; ++it) {
		float d = distance_sq(*it, ref);
		if(d < min_distance) {
			min_distance = d;
			closest_point = it;
		}
	}
	
	return closest_point;
}


}