#include <utility>

namespace pcf {

template<typename Point, typename Allocator> template<typename Other_cloud>
grid_point_cloud<Point, Allocator>::grid_point_cloud(Other_cloud&& pc, const Eigen::Vector3f cell_sz, const Allocator& alloc) :
super(std::forward<Other_cloud>(pc), true, alloc), cell_size_(cell_sz) {
	build_grid_();
}

template<typename Point, typename Allocator>
void grid_point_cloud<Point, Allocator>::build_grid_() {
	
}

}
