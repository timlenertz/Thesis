namespace pcf {


template<typename Point, typename Allocator> 
bool grid_point_cloud<Point, Allocator>::subspace::expand() {
	bool any_change = false;
	for(std::ptrdiff_t i = 0; i < 3; ++i) {
		if(origin[i] > 0) { --origin[i]; any_change = true; }
		if(extremity[i] < that->number_of_cells_[i] - 1) { ++extremity[i]; any_change = true; }
	}
	return any_change;
}


template<typename Point, typename Allocator> 
std::size_t grid_point_cloud<Point, Allocator>::subspace::size() const {
	std::size_t n = 0;
	std::ptrdiff_t i = that->index_for_cell_( cell_coordinates(origin[0], extremity[0], 0) );
	for(std::ptrdiff_t x = origin[0]; x < extremity[0]; ++x) {
		for(std::ptrdiff_t y = origin[1]; y < extremity[1]; ++y) {
			std::ptrdiff_t i_begin = i + origin[2] - 1;
			std::ptrdiff_t i_end = i + extremity[2];
			Point* p_begin = (i_begin == -1 ? that->begin() : that->cells_[i_begin]);
			Point* p_end = that->cells_[i_end];
			n += (p_end - p_begin);
		
			i += that->number_of_cells_[2];
		}
		i += that->number_of_cells_[1] * that->number_of_cells_[2];
	}
	return n;
}


template<typename Point, typename Allocator> 
bounding_box grid_point_cloud<Point, Allocator>::subspace::box() const {
	bounding_box b;
	for(std::ptrdiff_t i = 0; i < 3; ++i) {
		b.origin[i] = that->origin_[i] + (that->cell_length_ * origin[i]);
		b.extremity[i] = that->origin_[i] + (that->cell_length_ * extremity[i]);
	}
	return b;
}


}
