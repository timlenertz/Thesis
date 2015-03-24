#include <algorithm>
#include <cassert>
#include <iostream>

namespace pcf {

template<typename Point>
bool point_cloud_segment<Point>::contains(const Point& p) const {
	return (&p >= begin_) && (&p < end_);
}


template<typename Point>
bool point_cloud_segment<Point>::contains(const point_cloud_segment& seg) const {
	return contains(*seg.begin_) && contains(*(seg.end_ - 1));
}


template<typename Point>
bool point_cloud_segment<Point>::intersects(const point_cloud_segment& seg) const {
	return contains(*seg.begin_) || contains(*(seg.end_ - 1));
}


template<typename Point>
bounding_box point_cloud_segment<Point>::box(float ep) const {
	const float inf = INFINITY;
	Eigen::Vector4f mn(+inf, +inf, +inf, 0);
	Eigen::Vector4f mx(-inf, -inf, -inf, 0);

	
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
			mn = mn.cwiseMin(mn_part);
			mx = mx.cwiseMax(mx_part);
		}
	}
		
	mx += Eigen::Vector4f(ep, ep, ep, 0);
	
	return bounding_box(mn.head(3), mx.head(3));
}


template<typename Point>
Eigen::Vector3f point_cloud_segment<Point>::center_of_mass() const {
	std::size_t n = 0;
	Eigen::Vector4f sum = Eigen::Vector4f::Zero();
	for(auto&& p : *this) {
		if(! p.valid()) continue;
		sum += p.homogeneous_coordinates;
		++n;
	}
	return (sum / n).head(3);
}



template<typename Point> template<typename Index_func, typename Output_iterator>
void point_cloud_segment<Point>::partition_into_segments(Index_func idx, std::size_t n, Output_iterator it) const {
	point_cloud_segment cur = *this;
	Point* split = nullptr;
	for(std::size_t i = 0; i < n; ++i) {
		auto cond = [i, &idx](const Point& p) { return (idx(p) == i); };
		split = std::partition(cur.begin(), cur.end(), cond);
		*(it++) = point_cloud_segment(cur.begin(), split);
		cur.begin_ = split;
	}
	assert(split == end());
}


}