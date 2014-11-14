#ifndef PCF_GRID_POINT_CLOUD_SUBSPACE_H_
#define PCF_GRID_POINT_CLOUD_SUBSPACE_H_

#include <iterator>
#include <vector>
#include <stdexcept>
#include "../segment_union_iterator.h"

namespace pcf {

/**
Cuboid region of cells of a grid point cloud.
Represented in terms of origin and extremity cell coordinates. If origin=extremity, corresponds to one single cell.
Provides interator interface to ierate through cell coordinates (not points).
*/
template<typename Point, typename Allocator>
class grid_point_cloud<Point, Allocator>::subspace {
private:
	const grid_point_cloud& cloud_;

public:	
	class cell_iterator;

	cell_coordinates origin;
	cell_coordinates extremity;

	subspace(const grid_point_cloud& cl, const cell_coordinates& o, const cell_coordinates& e) :
		cloud_(cl), origin(o), extremity(e) { }
	
	subspace(const subspace&) = default;
	subspace& operator=(const subspace&);
	
	bool operator==(const subspace&) const;
	bool operator!=(const subspace&) const;
	
	const grid_point_cloud& cloud() const { return cloud_; }
		
	bool expand();
	std::size_t expand(std::size_t);
	std::size_t number_of_points() const; ///< Count number of points in subspace.
	bounding_box box() const;
	
	bool is_cubic() const;
	
	std::size_t size() const; ///< Number of cells in subspace.
	cell_iterator begin() const;
	cell_iterator end() const;
};



template<typename Point, typename Allocator>
class grid_point_cloud<Point, Allocator>::subspace::cell_iterator :
public std::iterator<std::forward_iterator_tag, cell_coordinates> {
private:
	const subspace& subspace_;
	cell_coordinates current_;
	
public:
	cell_iterator(const subspace& s, const cell_coordinates& c) : subspace_(s), current_(c) { }
	cell_iterator(const cell_iterator&) = default;
		
	cell_iterator& operator=(const cell_iterator& it) {
		assert(it.subspace_ == subspace_);
		current_ = it.current_;
		return *this;
	}
	
	bool operator==(const cell_iterator& it) const { return (it.current_ == current_); }
	bool operator!=(const cell_iterator& it) const { return (it.current_ != current_); }
	
	const cell_coordinates& operator*() const { return current_; }
	const cell_coordinates* operator->() const { return &current_; }
	
	cell_iterator& operator++();
	cell_iterator operator++(int) { cell_iterator old = *this; operator++(); return old; }
};


}

#include "subspace.tcc"

#endif