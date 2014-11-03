#include <cmath>

namespace pcf {

template<typename Point, typename Iterator>
Iterator find_closest_point(const Point& ref, Iterator begin, Iterator end) {
	float min_distance = INFINITY;
	Iterator closest_point = nullptr;
	
	#pragma omp parallel
	{
		float min_distance_part = INFINITY;
		Iterator closest_point_part = nullptr;

		#pragma omp for
		for(Iterator it = begin; it < end; ++it) {
			float d = euclidian_distance_sq(*it, ref);
			if(d < min_distance_part) {
				min_distance_part = d;
				closest_point_part = it;
			}
		}
		
		#pragma omp atomic
		if(min_distance_part < min_distance) {
			#pragma omp critical
			{
				min_distance = min_distance_part;
				closest_point = closest_point_part;
			}		
		}
	}
	
	return closest_point;
}


}