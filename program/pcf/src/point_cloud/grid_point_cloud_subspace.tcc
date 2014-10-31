#include <iterator>
#include <stdexcept>

namespace pcf {

/**
Cuboid region of cells of a grid point cloud.
Represented in terms of origin and extremity cell coordinates. If origin=extremity, corresponds to one single cell.
Provides iterator interface to visit all points contained in subspace. Remains connected to grid point cloud.
*/
template<typename Point, typename Allocator>
class grid_point_cloud<Point, Allocator>::subspace {
private:
	grid_point_cloud& cloud_;

public:
	cell_coordinates origin;
	cell_coordinates extremity;

	class cell_iterator;
	class iterator;
	
	grid_point_cloud& cloud() { return cloud_; }
	const grid_point_cloud& cloud() const { return cloud_; }
		
	bool expand();
	std::size_t size() const;
	bounding_box box() const;
	
	bool is_cubic() const;
	
	std::size_t number_of_cells() const;

	cell_iterator begin_cells();
	cell_iterator end_cells();
	
	iterator begin();
	iterator end();

	subspace(grid_point_cloud& cl, const cell_coordinates& o, const cell_coordinates& e) :
		cloud_(cl), origin(o), extremity(e) {}
	subspace(const subspace&) = default;
	subspace& operator=(const subspace&);
};

}

#include "grid_point_cloud_subspace_iterator.tcc"

namespace pcf {

template<typename Point, typename Allocator> 
auto grid_point_cloud<Point, Allocator>::subspace::operator=(const subspace& s) -> subspace& {
	assert(&cloud_ == &s.cloud_);
	origin = s.origin;
	extremity = s.extremity;
	return *this;
}


template<typename Point, typename Allocator> 
auto grid_point_cloud<Point, Allocator>::subspace::begin_cells() -> cell_iterator {
	return cell_iterator(*this, origin);
}


template<typename Point, typename Allocator> 
auto grid_point_cloud<Point, Allocator>::subspace::end_cells() -> cell_iterator {
	cell_iterator it(*this, extremity);
	return ++it;
}


template<typename Point, typename Allocator> 
auto grid_point_cloud<Point, Allocator>::subspace::begin() -> iterator {
	return iterator(begin_cells());
}


template<typename Point, typename Allocator> 
auto grid_point_cloud<Point, Allocator>::subspace::end() -> iterator {
	return iterator(end_cells());
}



template<typename Point, typename Allocator> 
std::size_t grid_point_cloud<Point, Allocator>::subspace::number_of_cells() const {
	std::size_t n = 1;
	for(std::ptrdiff_t i = 0; i < 3; ++i) n *= extremity[i] + 1 - origin[i];
	return n;
}



template<typename Point, typename Allocator> 
bool grid_point_cloud<Point, Allocator>::subspace::is_cubic() const {
	std::ptrdiff_t n[3];
	for(std::ptrdiff_t i = 0; i < 3; ++i) n[i] = extremity[i] - origin[i];
	return (n[0] == n[1]) && (n[1] == n[2]);
}


template<typename Point, typename Allocator> 
bool grid_point_cloud<Point, Allocator>::subspace::expand() {
	bool any_change = false;
	for(std::ptrdiff_t i = 0; i < 3; ++i) {
		if(origin[i] > 0) { --origin[i]; any_change = true; }
		if(extremity[i]+1 < cloud_.number_of_cells_[i]) { ++extremity[i]; any_change = true; }
	}
	return any_change;
}


template<typename Point, typename Allocator> 
std::size_t grid_point_cloud<Point, Allocator>::subspace::size() const {
	std::size_t n = 0;
	std::ptrdiff_t i = cloud_.index_for_cell_( cell_coordinates{origin[0], extremity[0], 0} );
	for(std::ptrdiff_t x = origin[0]; x < extremity[0]; ++x) {
		for(std::ptrdiff_t y = origin[1]; y < extremity[1]; ++y) {
			std::ptrdiff_t i_begin = i + origin[2] - 1;
			std::ptrdiff_t i_end = i + extremity[2];
			std::ptrdiff_t p_begin = (i_begin == -1 ? 0 : cloud_.cell_offsets_[i_begin]);
			std::ptrdiff_t p_end = cloud_.cell_offsets_[i_end];
			n += (p_end - p_begin);
		
			i += cloud_.number_of_cells_[2];
		}
		i += cloud_.number_of_cells_[1] * cloud_.number_of_cells_[2];
	}
	return n;
}


template<typename Point, typename Allocator> 
bounding_box grid_point_cloud<Point, Allocator>::subspace::box() const {
	bounding_box b;
	for(std::ptrdiff_t i = 0; i < 3; ++i) {
		b.origin[i] = cloud_.origin_[i] + (cloud_.cell_length_ * origin[i]);
		b.extremity[i] = cloud_.origin_[i] + (cloud_.cell_length_ * (extremity[i]+1));
	}
	return b;
}


}
