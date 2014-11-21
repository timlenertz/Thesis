#include <cmath>
#include <memory>
#include <algorithm>
#include "../util/alignment.h"
#include "../util/multi_dimensional_array.h"
#include "../point_algorithm.h"

namespace pcf {

template<typename Point, typename Allocator>
void point_cloud<Point, Allocator>::check_correct_alignment_() const {
	if((std::uintptr_t)begin_ % ALIGNOF(Point))
		throw std::runtime_error("Point cloud data not properly aligned.");
}

template<typename Point, typename Allocator>
void point_cloud<Point, Allocator>::resize_(std::size_t new_size) {	
	if(new_size > capacity()) throw std::logic_error("Point cloud new size cannot be greater than capacity.");
	else end_ = begin_ + new_size;
}


template<typename Point, typename Allocator>
void point_cloud<Point, Allocator>::initialize_() {
	for(Point& p : *this) p.invalidate();
}


template<typename Point, typename Allocator>
const Point& point_cloud<Point, Allocator>::invalid_point_() {	
	static const Point invalid = Point();
	return invalid;
}


template<typename Point, typename Allocator>
point_cloud<Point, Allocator>::point_cloud(std::size_t allocate_size, bool all_val, const Allocator& alloc) :
	allocator_(alloc),
	allocated_size_(allocate_size),
	all_valid_(all_val),
	begin_( allocator_.allocate(allocate_size) ),
	end_( begin_ )
{
	check_correct_alignment_();
}



template<typename Point, typename Allocator>
point_cloud<Point, Allocator>::
point_cloud(point_cloud&& pc, bool all_val) :
	allocator_( std::move(pc.allocator_) ),
	allocated_size_(pc.allocated_size_),
	all_valid_(all_val),
	begin_( pc.begin_ ),
	end_( pc.end_ )
{
	pc.begin_ = nullptr;
}


template<typename Point, typename Allocator>
point_cloud<Point, Allocator>::~point_cloud() {
	if(begin_) allocator_.deallocate(begin_, allocated_size_);
}




template<typename Point, typename Allocator>
std::size_t point_cloud<Point, Allocator>::number_of_valid_points() const {
	if(all_valid_) {
		return size();
	} else {
		std::size_t total = 0;
		#pragma omp parallel for reduction(+:total)
		for(auto p = begin(); p < end(); ++p) if(p->valid()) ++total;
		return total;
	}
}

template<typename Point, typename Allocator>
bool point_cloud<Point, Allocator>::contains_invalid_points() const {
	if(all_valid_) return false;
	for(const Point& p : *this) if(! p.valid()) return false;
	return true;
}


template<typename Point, typename Allocator> template<typename Writer>
void point_cloud<Point, Allocator>::write(Writer& writer) const {
	if(all_valid_) {
		writer.write(data(), size());
	} else {
		const Point* segment_start = nullptr;
		for(const Point* p = begin(); p != end(); ++p) {
			bool valid = p->valid();
			if(!valid && segment_start) {
				writer.write(segment_start, p - segment_start);
				segment_start = nullptr;
			} else if(valid && !segment_start) {
				segment_start = p;
			}
		}
		if(segment_start) writer.write(segment_start, end() - segment_start);
	}
}


}