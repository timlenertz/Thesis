#ifndef PCF_GRID_POINT_CLOUD_H_
#define PCF_GRID_POINT_CLOUD_H_

#include <utility>
#include <functional>
#include <memory>
#include <array>
#include <ostream>
#include <cmath>
#include "../point_cloud.h"
#include "../../geometry/bounding_box.h"
#include "../../util/multi_dimensional_array.h"

namespace pcf {

/**
Point cloud divided into a regular grid of cubic cells.
All cells are same size, with side length defined upon construction. Cells are refered to
by cell coordinates, (0, 0, 0) being the origin starting at the cloud's bounding box origin.
Data is segmented into one segment per cell. Interface for iterating through points in cell
or group of cells is provided by grid_point_cloud_subspace.
Implements kNN algorithm. [Zhao2014]
*/
template<typename Point, typename Allocator = default_allocator<Point>>
class grid_point_cloud : public point_cloud<Point, Allocator> {
	using super = point_cloud<Point, Allocator>;
	
public:
	using segment = typename super::segment;
	using const_segment = typename super::const_segment;
	using segment_union = typename super::segment_union;
	using const_segment_union = typename super::const_segment_union;

	using cell_coordinates = array_3dim<std::ptrdiff_t>::indices_type;
	class subspace;

private:
	const float cell_length_;
	const bounding_box box_;
	array_3dim<std::ptrdiff_t> cells_;

	template<typename Callback_func> void iterate_cells_(Callback_func callback, bool parallel) const;
	template<typename Callback_func> void iterate_cells_(Callback_func callback, bool parallel);
	
	array_3dim<std::ptrdiff_t>::sizes_type cells_array_sizes_() const;
	void build_grid_();
	
	template<typename That, typename Condition_func, typename Callback_func>
	static void nearest_neighbors_(That that, std::size_t k, Condition_func cond, Callback_func callback, bool parallel);

public:	
	template<typename Other_cloud>
	grid_point_cloud(const Other_cloud& pc, float cell_len = 0, const Allocator& alloc = Allocator());

	grid_point_cloud(super&&, float cell_len = 0);

	grid_point_cloud(const grid_point_cloud& pc) : grid_point_cloud(pc, pc.cell_length_) { }
	grid_point_cloud(grid_point_cloud&& pc) : grid_point_cloud(std::move(pc), pc.cell_length_) { }
	
	bool verify() const;
	
	void test_colorize();
	
	template<typename Other_point>
	const Point& closest_point(const Other_point& from, float accepting_distance = 0, float rejecting_distance = INFINITY) const;
	
	template<typename Condition_func, typename Callback_func>
	void nearest_neighbors(std::size_t k, Condition_func cond, Callback_func callback, bool parallel = true) const {
		nearest_neighbors_(this, k, cond, callback, parallel);	
	}
	
	template<typename Condition_func, typename Callback_func>
	void nearest_neighbors(std::size_t k, Condition_func cond, Callback_func callback, bool parallel = true) {
		nearest_neighbors_(this, k, cond, callback, parallel);	
	}
	
	
	std::size_t number_of_cells() const { return cells_.total_size(); }
	std::size_t number_of_cells(std::ptrdiff_t i) const { return cells_.size(i); }
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

template<typename Cloud>
float default_grid_cell_length(const Cloud& pc);


using grid_point_cloud_xyz = grid_point_cloud<point_xyz>;
using grid_point_cloud_full = grid_point_cloud<point_full>;

extern template class grid_point_cloud<point_xyz>;
extern template class grid_point_cloud<point_full>;

}

#include "subspace.h"
#include "grid_point_cloud.tcc"

#endif