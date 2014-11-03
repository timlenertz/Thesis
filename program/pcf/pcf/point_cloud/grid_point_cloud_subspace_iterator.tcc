#include <iterator>

namespace pcf {

template<typename Point, typename Allocator>
class grid_point_cloud<Point, Allocator>::subspace::cell_iterator :
public std::iterator<std::input_iterator_tag, cell_coordinates> {
private:
	subspace& subspace_;
	cell_coordinates current_;
	
public:
	cell_iterator(subspace& s, const cell_coordinates& c) : subspace_(s), current_(c) { }
	cell_iterator(const cell_iterator&) = default;
	
	grid_point_cloud& cloud() { return subspace_.cloud(); }
	
	cell_iterator& operator=(const cell_iterator& it) {
		if(&(it.subspace_) != &subspace_)
			throw std::invalid_argument("Cannot assign cell iterator associated to different subspace.");
		current_ = it.current_;
		return *this;
	}
	
	bool operator==(const cell_iterator& it) const { return (it.current_ == current_); }
	bool operator!=(const cell_iterator& it) const { return (it.current_ != current_); }
	
	const cell_coordinates& operator* () const { return current_; }
	const cell_coordinates* operator-> () const { return &current_; }
	
	cell_iterator& operator++();
	
	cell_iterator operator++(int) {
		cell_iterator old = *this;
		operator++();
		return old;
	}
};



template<typename Point, typename Allocator>
class grid_point_cloud<Point, Allocator>::subspace::iterator :
public std::iterator<std::input_iterator_tag, Point> {
private:
	cell_iterator cell_it_;
	Point* current_;
	Point* cell_end_;
	
	void load_cell_();
	void load_next_cell_();
	
public:
	explicit iterator(const cell_iterator& cit);
	iterator(const iterator&) = default;
	iterator& operator=(const iterator&) = default;
	
	bool operator==(const iterator& it) const { return (it.current_ == current_); }
	bool operator!=(const iterator& it) const { return (it.current_ != current_); }
	
	cell_coordinates current_cell() const { return *cell_it_; }
	
	Point& operator* () const { return *current_; }
	Point* operator-> () const { return current_; }
	
	iterator& operator++();
	
	iterator operator++(int) {
		iterator old = *this;
		operator++();
		return old;
	}
};


template<typename Point, typename Allocator>
grid_point_cloud<Point, Allocator>::subspace::iterator::iterator(const cell_iterator& cit) :
cell_it_(cit) {
	load_cell_();
}



template<typename Point, typename Allocator>
auto grid_point_cloud<Point, Allocator>::subspace::cell_iterator::operator++() -> cell_iterator& {
	if(current_[2] < subspace_.extremity_[2]) {
		++current_[2];
	} else if(current_[1] < subspace_.extremity_[1]) {
		current_[2] = subspace_.origin_[2];
		++current_[1];
	} else {
		current_[2] = subspace_.origin_[2];
		current_[1] = subspace_.origin_[1];
		++current_[0];
	}
	return *this;
}



template<typename Point, typename Allocator>
void grid_point_cloud<Point, Allocator>::subspace::iterator::load_cell_() {
	bool continue_to_next_cell;
	do {
		continue_to_next_cell = false;
		
		// Get index of current cell in cloud
		std::ptrdiff_t i = cell_it_.cloud().index_for_cell_(*cell_it_);

		if(i < cell_it_.cloud().cell_offsets_.size()) {
			// Cell still in cloud, load segment start and end
			auto seg = cell_it_.cloud().segment_for_index_(i);
			current_ = seg.begin();
			cell_end_ = seg.end();
	
			if(current_ == cell_end_) {
				// Segment is empty. Need to advance to next cell (recursive)
				++cell_it_;
				continue_to_next_cell = true;
			}

		} else {
			// Cell beyond cloud end.
			current_ = cell_it_.cloud().end(); // Corresponds to end iterator
			cell_end_ = current_ - 1; // avoids endless loop in operator++
		}
		
	} while(continue_to_next_cell);
}


template<typename Point, typename Allocator>
void grid_point_cloud<Point, Allocator>::subspace::iterator::load_next_cell_() {
	++cell_it_;
	load_cell_();
}


template<typename Point, typename Allocator>
auto grid_point_cloud<Point, Allocator>::subspace::iterator::operator++() -> iterator& {
	if(++current_ == cell_end_) load_next_cell_();
	return *this;
}

}