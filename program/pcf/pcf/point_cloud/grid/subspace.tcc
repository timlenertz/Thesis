namespace pcf {

template<typename Point, typename Allocator> 
auto grid_point_cloud<Point, Allocator>::subspace::operator=(const subspace& s) -> subspace& {
	assert(&cloud_ == &s.cloud_);
	origin = s.origin;
	extremity = s.extremity;
	return *this;
}


template<typename Point, typename Allocator> 
bool grid_point_cloud<Point, Allocator>::subspace::operator==(const subspace& s) const {
	assert(&cloud_ == &s.cloud_);
	return (s.origin == origin) && (s.extremity == extremity);
}


template<typename Point, typename Allocator> 
bool grid_point_cloud<Point, Allocator>::subspace::operator!=(const subspace& s) const {
	assert(&cloud_ == &s.cloud_);
	return (s.origin != origin) || (s.extremity != extremity);
}



template<typename Point, typename Allocator> 
std::size_t grid_point_cloud<Point, Allocator>::subspace::size() const {
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
std::size_t grid_point_cloud<Point, Allocator>::subspace::expand(std::size_t n) {
	std::size_t expanded_times = 0;
	while(expanded_times < n && expand()) ++expanded_times;
	return expanded_times;
}


template<typename Point, typename Allocator> 
std::size_t grid_point_cloud<Point, Allocator>::subspace::number_of_points() const {
	cell_coordinates c(0, 0, 0);
	std::size_t sz = 0;
	std::ptrdiff_t i = 0;
	for(c[0] = origin[0]; c[0] <= extremity[0]; ++c[0]) for(c[1] = origin[1]; c[1] <= extremity[1]; ++c[1]) {
		std::ptrdiff_t i_begin = i + origin[2] - 1;
		std::ptrdiff_t i_end = i + extremity[2];

		std::ptrdiff_t off_begin = (i_begin == -1 ? 0 : cloud_.cell_offsets_[i_begin]);
		std::ptrdiff_t off_end = cloud_.cell_offsets_[i_end];
		sz += off_end - off_begin;
	
		i += cloud_.number_of_cells_[2];
	}
	return sz;
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


template<typename Point, typename Allocator>
inline auto grid_point_cloud<Point, Allocator>::subspace::begin() const -> cell_iterator {
	return cell_iterator(*this, origin);
}


template<typename Point, typename Allocator>
inline auto grid_point_cloud<Point, Allocator>::subspace::end() const -> cell_iterator {
	return cell_iterator(*this, extremity);
}



template<typename Point, typename Allocator>
auto grid_point_cloud<Point, Allocator>::subspace::cell_iterator::operator++() -> cell_iterator& {
	if(current_[2] < subspace_.extremity[2]) {
		++current_[2];
	} else if(current_[1] < subspace_.extremity[1]) {
		current_[2] = subspace_.origin[2];
		++current_[1];
	} else {
		current_[2] = subspace_.origin[2];
		current_[1] = subspace_.origin[1];
		++current_[0];
	}
	return *this;
}


}