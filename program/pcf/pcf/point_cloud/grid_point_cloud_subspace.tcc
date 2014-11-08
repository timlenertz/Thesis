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
public:
	class cell_iterator;
	class iterator;

private:
	grid_point_cloud& cloud_;
	cell_coordinates origin_;
	cell_coordinates extremity_;

	mutable iterator begin_iterator_;
	mutable iterator end_iterator_;
	bool need_to_update_iterators_;

public:	
	subspace(grid_point_cloud& cl, const cell_coordinates& o, const cell_coordinates& e);
	subspace(const subspace&) = default;
	subspace& operator=(const subspace&);
	
	bool operator==(const subspace&) const;
	bool operator!=(const subspace&) const;

	const cell_coordinates& get_origin() const { return origin_; }
	const cell_coordinates& get_extremity() const { return extremity_; }
	void set_origin(const cell_coordinates&);
	void set_extremity(const cell_coordinates&);
	
	grid_point_cloud& cloud() { return cloud_; }
	const grid_point_cloud& cloud() const { return cloud_; }
		
	bool expand();
	std::size_t expand(std::size_t);
	std::size_t size() const;
	bounding_box box() const;
	
	bool is_cubic() const;
	
	std::size_t number_of_cells() const;

	cell_iterator begin_cells();
	cell_iterator end_cells();
	
	iterator begin();
	iterator end();
};

}

#include "grid_point_cloud_subspace_iterator.tcc"

namespace pcf {

template<typename Point, typename Allocator> 
grid_point_cloud<Point, Allocator>::subspace::subspace
(grid_point_cloud& cl, const cell_coordinates& o, const cell_coordinates& e) :
cloud_(cl), origin_(o), extremity_(e), begin_iterator_(begin_cells()), end_iterator_(end_cells()), need_to_update_iterators_(false) { }



template<typename Point, typename Allocator> 
auto grid_point_cloud<Point, Allocator>::subspace::operator=(const subspace& s) -> subspace& {
	assert(&cloud_ == &s.cloud_);
	origin_ = s.origin;
	extremity_ = s.extremity;
	need_to_update_iterators_ = true;
	return *this;
}


template<typename Point, typename Allocator> 
bool grid_point_cloud<Point, Allocator>::subspace::operator==(const subspace& s) const {
	assert(&cloud_ == &s.cloud_);
	return (s.origin_ == origin_) && (s.extremity_ == extremity_);
}


template<typename Point, typename Allocator> 
bool grid_point_cloud<Point, Allocator>::subspace::operator!=(const subspace& s) const {
	assert(&cloud_ == &s.cloud_);
	return (s.origin_ != origin_) || (s.extremity_ != extremity_);
}


template<typename Point, typename Allocator> 
auto grid_point_cloud<Point, Allocator>::subspace::begin_cells() -> cell_iterator {
	return cell_iterator(*this, origin_);
}


template<typename Point, typename Allocator> 
auto grid_point_cloud<Point, Allocator>::subspace::end_cells() -> cell_iterator {
	cell_iterator it(*this, extremity_);
	return ++it;
}


template<typename Point, typename Allocator> 
auto grid_point_cloud<Point, Allocator>::subspace::begin() -> iterator {
	if(need_to_update_iterators_) {
		begin_iterator_ = iterator(begin_cells());
		need_to_update_iterators_ = false;
	}
	return begin_iterator_;
}


template<typename Point, typename Allocator> 
auto grid_point_cloud<Point, Allocator>::subspace::end() -> iterator {
	if(need_to_update_iterators_) {
		end_iterator_ = iterator(end_cells());
		need_to_update_iterators_ = false;
	}
	return end_iterator_;
}


template<typename Point, typename Allocator> 
void grid_point_cloud<Point, Allocator>::subspace::set_origin(const cell_coordinates& c) {
	origin_ = c;
	need_to_update_iterators_ = true;
}

template<typename Point, typename Allocator> 
void grid_point_cloud<Point, Allocator>::subspace::set_extremity(const cell_coordinates& c) {
	extremity_ = c;
	need_to_update_iterators_ = true;
}


template<typename Point, typename Allocator> 
std::size_t grid_point_cloud<Point, Allocator>::subspace::number_of_cells() const {
	std::size_t n = 1;
	for(std::ptrdiff_t i = 0; i < 3; ++i) n *= extremity_[i] + 1 - origin_[i];
	return n;
}



template<typename Point, typename Allocator> 
bool grid_point_cloud<Point, Allocator>::subspace::is_cubic() const {
	std::ptrdiff_t n[3];
	for(std::ptrdiff_t i = 0; i < 3; ++i) n[i] = extremity_[i] - origin_[i];
	return (n[0] == n[1]) && (n[1] == n[2]);
}


template<typename Point, typename Allocator> 
bool grid_point_cloud<Point, Allocator>::subspace::expand() {
	bool any_change = false;
	for(std::ptrdiff_t i = 0; i < 3; ++i) {
		if(origin_[i] > 0) { --origin_[i]; any_change = true; }
		if(extremity_[i]+1 < cloud_.number_of_cells_[i]) { ++extremity_[i]; any_change = true; }
	}
	if(any_change) need_to_update_iterators_ = true;
	return any_change;
}


template<typename Point, typename Allocator> 
std::size_t grid_point_cloud<Point, Allocator>::subspace::expand(std::size_t n) {
	std::size_t expanded_times = 0;
	while(expanded_times < n && expand()) ++expanded_times;
	return expanded_times;
}


template<typename Point, typename Allocator> 
std::size_t grid_point_cloud<Point, Allocator>::subspace::size() const {
	std::size_t n = 0;
	std::ptrdiff_t i = cloud_.index_for_cell_( cell_coordinates{origin_[0], extremity_[0], 0} );
	for(std::ptrdiff_t x = origin_[0]; x < extremity_[0]; ++x) {
		for(std::ptrdiff_t y = origin_[1]; y < extremity_[1]; ++y) {
			std::ptrdiff_t i_begin = i + origin_[2] - 1;
			std::ptrdiff_t i_end = i + extremity_[2];
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
		b.origin[i] = cloud_.origin_[i] + (cloud_.cell_length_ * origin_[i]);
		b.extremity[i] = cloud_.origin_[i] + (cloud_.cell_length_ * (extremity_[i]+1));
	}
	return b;
}


}
