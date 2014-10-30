#ifndef PCF_GRID_POINT_CLOUD_H_
#define PCF_GRID_POINT_CLOUD_H_

#include <utility>
#include <functional>
#include <memory>
#include <array>
#include "point_cloud.h"
#include "../geometry/bounding_box.h"

namespace pcf {

template<typename Point, typename Allocator = aligned_allocator<Point>>
class grid_point_cloud : public point_cloud<Point, Allocator> {
	using super = point_cloud<Point, Allocator>;
	using segment = typename super::segment;
	
private:
	using cell_coordinates = std::array<std::ptrdiff_t, 3>;
	class subspace;

	const float cell_length_;

	Eigen::Vector3f origin_;
	std::size_t number_of_cells_[3];
	std::vector<std::ptrdiff_t> cell_offsets_;

	segment segment_for_index_(std::ptrdiff_t i);
	const segment segment_for_index_(std::ptrdiff_t i) const;
	
	template<typename Other_point> cell_coordinates cell_for_point_(const Other_point&) const;
	std::ptrdiff_t index_for_cell_(const cell_coordinates&) const;
	
	subspace make_subspace(const cell_coordinates& c) {
		return subspace(this, c);
	}
	
	template<typename Callback_func> void iterate_cells_(Callback_func callback, bool parallel = true) const;
	
	bool in_bounds_(const cell_coordinates&) const;
	void build_grid_();

public:
	template<typename Other_cloud> grid_point_cloud(Other_cloud&& pc, float cell_len, const Allocator& alloc);
	template<typename Other_cloud> grid_point_cloud(Other_cloud&& pc, float cell_len);
	
	
	
	bool verify() const;
	
	template<typename Other_point>
	const Point& find_closest_point(const Other_point& from) const;
	
	template<typename Callback_func>
	void find_nearest_neighbors(std::size_t k, Callback_func callback) const;
	
	std::size_t number_of_cells() const { return cell_offsets_.size(); }
	std::size_t number_of_empty_cells() const;
};




template<typename Point, typename Allocator>
class grid_point_cloud<Point, Allocator>::subspace {
private:
	grid_point_cloud* that;

public:
	cell_coordinates origin;
	cell_coordinates extremity;
	
	bool expand();
	std::size_t size() const;
	bounding_box box() const;

	subspace(grid_point_cloud* t, const cell_coordinates& c) : that(t), origin(c), extremity(c) { }
};

}

#include "grid_point_cloud_subspace.tcc"
#include "grid_point_cloud.tcc"

#endif