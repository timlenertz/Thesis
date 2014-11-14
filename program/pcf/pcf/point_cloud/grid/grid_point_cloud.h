#ifndef PCF_GRID_POINT_CLOUD_H_
#define PCF_GRID_POINT_CLOUD_H_

#include <utility>
#include <functional>
#include <memory>
#include <array>
#include <ostream>
#include "../point_cloud.h"
#include "../../geometry/bounding_box.h"

namespace pcf {

/**
Point cloud divided into a regular grid of cubic cells.
All cells are same size, with side length defined upon construction. Cells are refered to
by cell coordinates, (0, 0, 0) being the origin starting at the cloud's bounding box origin.
Data is segmented into one segment per cell. Interface for iterating through points in cell
or group of cells is provided by grid_point_cloud_subspace.
Implements kNN algorithm. [Zhao2014]
*/
template<typename Point, typename Allocator = aligned_allocator<Point>>
class grid_point_cloud : public point_cloud<Point, Allocator> {
	using super = point_cloud<Point, Allocator>;
	
public:
	using segment = typename super::segment;
	using const_segment = typename super::const_segment;
	using segment_union = typename super::segment_union;
	using const_segment_union = typename super::const_segment_union;

	class cell_coordinates;
	class subspace;

private:
	const float cell_length_;

	Eigen::Vector3f origin_;
	std::size_t number_of_cells_[3];
	std::vector<std::ptrdiff_t> cell_offsets_;

	segment segment_for_index_(std::ptrdiff_t i);
	const_segment segment_for_index_(std::ptrdiff_t i) const;
	
	std::ptrdiff_t index_for_cell_(const cell_coordinates&) const;
	
	template<typename Callback_func> void iterate_cells_(Callback_func callback, bool parallel = true) const;
	
	bool in_bounds_(const cell_coordinates&) const;
	void move_into_bounds_(cell_coordinates&) const;
	
	void build_grid_();

public:	
	template<typename Other_cloud> grid_point_cloud(Other_cloud&& pc, float cell_len, const Allocator& alloc);
	template<typename Other_cloud> grid_point_cloud(Other_cloud&& pc, float cell_len);
	
	bool verify() const;
	
	template<typename Other_point>
	const Point& find_closest_point(const Other_point& from) const;
	
	template<typename Condition_func, typename Callback_func>
	void find_nearest_neighbors(std::size_t k, Condition_func cond, Callback_func callback, bool parallel = false) const;
	
	std::size_t number_of_cells() const { return cell_offsets_.size(); }
	std::size_t number_of_cells(std::ptrdiff_t i) const { return number_of_cells_[i]; }
	std::size_t number_of_empty_cells() const;
	
	subspace full_subspace() const;
	subspace cell_subspace(const cell_coordinates&) const;
	
	template<typename Other_point> cell_coordinates cell_for_point(const Other_point&) const;
	
	segment segment_for_cell(const cell_coordinates&);
	const_segment segment_for_cell(const cell_coordinates&) const;
	segment_union segment_union_for_subspace(const subspace&);
	const_segment_union segment_union_for_subspace(const subspace&) const;
};


template<typename Cloud>
float optimal_grid_cell_length_for_knn(const Cloud& pc, std::size_t k, float alpha);


/**
Coordinates of a cell in the grid point cloud.
*/
template<typename Point, typename Allocator>
class grid_point_cloud<Point, Allocator>::cell_coordinates {
private:
	std::array<std::ptrdiff_t, 3> c_;
	
public:
	cell_coordinates() { c_ = {0, 0, 0}; }
	cell_coordinates(std::ptrdiff_t x, std::ptrdiff_t y, std::ptrdiff_t z) { c_ = {x, y, z}; }
	cell_coordinates(const cell_coordinates&) = default;
	
	std::ptrdiff_t& operator[](std::ptrdiff_t i) { return c_[i]; }
	const std::ptrdiff_t& operator[](std::ptrdiff_t i) const { return c_[i]; }
	
	bool operator==(const cell_coordinates& c) const { return c_ == c.c_; }
	bool operator!=(const cell_coordinates& c) const { return c_ != c.c_; }
		
	friend std::ostream& operator<<(std::ostream& str, const cell_coordinates& c) {
		str << "(" << c.c_[0] << ", " << c.c_[1] << ", " << c.c_[2] << ")";
		return str;
	}
};

using grid_point_cloud_xyz = grid_point_cloud<point_xyz>;
using grid_point_cloud_full = grid_point_cloud<point_full>;

}

#include "subspace.h"
#include "grid_point_cloud.tcc"

#endif