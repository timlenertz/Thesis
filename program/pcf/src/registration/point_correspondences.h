#ifndef PCF_POINT_CORRESPONDENCES_H_
#define PCF_POINT_CORRESPONDENCES_H_

#include <vector>
#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include "../point.h"

namespace pcf {

template<typename... Point>
class point_correspondences {
public:
	constexpr static std::size_t number_of_point_clouds = sizeof...(Point);

	struct correspondence {
		std::tuple<const Point&...> points;
		float weight = 1.0;
		
		correspondence(const Point&... pts) : points(pts...) { }
	};

private:
	std::vector<correspondence> correspondences_;
	
public:
	void erase() { correspondences_.clear(); }
	
	template<typename Cloud1, typename Cloud2>
	void find_closest_points(const Cloud1& pc1, const Cloud2& pc2) {
		#pragma omp parallel for
		for(auto p1 = pc1.cbegin(); p1 < pc1.cend(); ++p1) {
			const auto& p2 = pc2.find_closest_point(*p1, euclidian_distance_sq);
			#pragma omp critical
			{ correspondences_.emplace_back(*p1, p2); }
		}
	}
	
	float error() const {
		float sum = 0.0;
		#pragma omp parallel for reduction(+:sum)
		for(auto it = correspondences_.cbegin(); it < correspondences_.cend(); ++it) {
			sum += euclidian_distance_sq(std::get<0>(it->points), std::get<1>(it->points));
		}
		return sum;
	}
};

}

#endif