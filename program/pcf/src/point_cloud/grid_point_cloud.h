#ifndef PCF_GRID_POINT_CLOUD_H_
#define PCF_GRID_POINT_CLOUD_H_

#include <utility>
#include <functional>
#include <memory>
#include <array>
#include <ostream>
#include "point_cloud.h"
#include "../geometry/bounding_box.h"

namespace pcf {

template<typename Point, typename Allocator = aligned_allocator<Point>>
class grid_point_cloud : public point_cloud<Point, Allocator> {
	using super = point_cloud<Point, Allocator>;
	using segment = typename super::segment;
	
public:
	class cell_coordinates;
	class subspace;

private:
	const float cell_length_;

	Eigen::Vector3f origin_;
	std::size_t number_of_cells_[3];
	std::vector<std::ptrdiff_t> cell_offsets_;

	segment segment_for_index_(std::ptrdiff_t i);
	const segment segment_for_index_(std::ptrdiff_t i) const;
	
	template<typename Other_point> cell_coordinates cell_for_point_(const Other_point&) const;
	std::ptrdiff_t index_for_cell_(const cell_coordinates&) const;
	
	template<typename Callback_func> void iterate_cells_(Callback_func callback, bool parallel = true) const;
	
	bool in_bounds_(const cell_coordinates&) const;
	void build_grid_();

public:
	template<typename Other_cloud>
	static float optimal_cell_length_for_knn(const Other_cloud& pc, std::size_t k, float alpha);
	
	template<typename Other_cloud> grid_point_cloud(Other_cloud&& pc, float cell_len, const Allocator& alloc);
	template<typename Other_cloud> grid_point_cloud(Other_cloud&& pc, float cell_len);
	
	bool verify() const;
	
	template<typename Other_point>
	const Point& find_closest_point(const Other_point& from) const;
	
	template<typename Callback_func>
	void find_nearest_neighbors(std::size_t k, Callback_func callback) const;
	
	std::size_t number_of_cells() const { return cell_offsets_.size(); }
	std::size_t number_of_cells(std::ptrdiff_t i) const { return number_of_cells_[i]; }
	std::size_t number_of_empty_cells() const;
	
	subspace full_subspace();
	subspace cell_subspace(const cell_coordinates& c);
};



template<typename Point, typename Allocator>
class grid_point_cloud<Point, Allocator>::cell_coordinates {
private:
	std::ptrdiff_t c_[3];
	
public:
	cell_coordinates() : c_{0, 0, 0} { }
	cell_coordinates(std::ptrdiff_t x, std::ptrdiff_t y, std::ptrdiff_t z) : c_{x, y, z} { }
	cell_coordinates(const cell_coordinates&) = default;
	
	std::ptrdiff_t& operator[](std::ptrdiff_t i) { return c_[i]; }
	const std::ptrdiff_t& operator[](std::ptrdiff_t i) const { return c_[i]; }
	
	bool operator==(const cell_coordinates& c) const
		{ return (c_[0] == c.c_[0]) && (c_[1] == c.c_[1]) && (c_[2] == c.c_[2]); }
	bool operator!=(const cell_coordinates& c) const
		{ return (c_[0] != c.c_[0]) || (c_[1] != c.c_[1]) || (c_[2] != c.c_[2]); }
		
	friend std::ostream& operator<<(std::ostream& str, const cell_coordinates& c) {
		str << "(" << c.c_[0] << ", " << c.c_[1] << ", " << c.c_[2] << ")";
		return str;
	}
};


}

#include "grid_point_cloud_subspace.tcc"
#include "grid_point_cloud.tcc"

#endif