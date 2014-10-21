#include <algorithm>

namespace pcf {

template<typename Point>
cuboid point_cloud_segment<Point>::bounding_cuboid(float ep) const {
	const float inf = INFINITY;
	cuboid cub(
		Eigen::Vector3f(+inf, +inf, +inf),
		Eigen::Vector3f(-inf, -inf, -inf)
	);
	
	#pragma omp parallel
	{
		Eigen::Vector4f mn_part(+inf, +inf, +inf, 0);
		Eigen::Vector4f mx_part(-inf, -inf, -inf, 0);
		
		#pragma omp for
		for(Point* p = begin_; p < end_; ++p) {
			if(! p->valid()) continue;
			const Eigen::Vector4f pc = p->homogeneous_coordinates;
			
			mn_part = mn_part.cwiseMin(pc);
			mx_part = mx_part.cwiseMax(pc);
		}
		
		#pragma omp critical
		{
			cub.origin = cub.origin.cwiseMin(mn_part.head(3));
			cub.extremity = cub.extremity.cwiseMax(mx_part.head(3));
		}
	}
		
	cub.extremity += Eigen::Vector3f(ep, ep, ep);
	
	return cub;
}


template<typename Point>
Eigen::Vector3f point_cloud_segment<Point>::center_of_mass() const {
	Eigen::Vector4f sum = Eigen::Vector4f::Zero();
	std::size_t total = size();
	
	#pragma omp parallel
	{
		Eigen::Vector4f sum_part = Eigen::Vector4f::Zero();
		
		total = 0;
		#pragma omp for reduction(+:total)
		for(Point* p = begin_; p < end_; ++p) {
			if(! p->valid()) continue;
			sum_part += p->homogeneous_coordinates;
			++total;
		}
	
		#pragma omp critical
		{ sum += sum_part; }
	}

	return (sum / total).head(3);
}



template<typename Point> template<typename Index_func, typename Output_iterator>
void point_cloud_segment<Point>::partition_into_segments(Index_func idx, std::size_t n, Output_iterator it) {
	point_cloud_segment cur = *this;
	for(std::size_t i = 0; i < n; ++i) {
		auto cond = [i, &idx](const Point& p) { return (idx(p) == i); };
		Point* split = std::partition(cur.begin(), cur.end(), cond);
		*(it++) = point_cloud_segment(cur.begin(), split);
		cur.begin_ = split;
	}
	*it = cur;
}


}