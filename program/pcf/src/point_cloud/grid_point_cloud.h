#ifndef PCF_GRID_POINT_CLOUD_H_
#define PCF_GRID_POINT_CLOUD_H_

#include <utility>
#include <functional>
#include <memory>
#include <array>
#include "point_cloud.h"
#include "../geometry/cuboid.h"

namespace pcf {

template<typename Point, typename Allocator = std::allocator<Point>>
class grid_point_cloud : public point_cloud<Point, Allocator> {
	using super = point_cloud<Point, Allocator>;
	using segment = typename super::segment;
	
private:
	using cell_coordinates = std::array<std::ptrdiff_t, 3>;

	const Eigen::Vector3f cell_size_;

	Eigen::Vector3f origin_;
	std::size_t number_of_cells_[3];
	std::vector<segment> cells_;
	
	template<typename Other_point> cell_coordinates cell_for_point_(const Other_point&) const;
	std::ptrdiff_t index_for_cell_(const cell_coordinates&) const;
	
	bool in_bounds_(const cell_coordinates&) const;
	void build_grid_();

public:
	template<typename Other_cloud> grid_point_cloud(Other_cloud&& pc, const Eigen::Vector3f cell_sz, const Allocator& alloc);
	template<typename Other_cloud> grid_point_cloud(Other_cloud&& pc, const Eigen::Vector3f cell_sz);
	
	void verify() const;
	
	template<typename Other_point>
	const Point& find_closest_point(const Other_point& from) const;
};

}

#include "grid_point_cloud.tcc"

#endif