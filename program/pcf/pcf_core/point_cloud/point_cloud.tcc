#include <cmath>
#include <memory>
#include <algorithm>
#include "../util/memory.h"
#include "../util/random.h"
#include "../util/multi_dimensional_array.h"
#include "../point_algorithm.h"
#include <iostream>

namespace pcf {

template<typename Point, typename Allocator>
void point_cloud<Point, Allocator>::check_correct_alignment_() const {
	/*if((std::uintptr_t)begin_ % ALIGNOF(Point))
		throw std::runtime_error("Point cloud data not properly aligned.");*/
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
	space_object(pc),
	allocator_( std::move(pc.allocator_) ),
	allocated_size_(pc.allocated_size_),
	all_valid_(all_val),
	begin_( pc.begin_ ),
	end_( pc.end_ )
{
	check_correct_alignment_();
	pc.begin_ = nullptr;
	
	// If this is all valid but other is not, remove invalid points from buffer.
	if(all_val && ! pc.all_valid())
		end_ = std::partition(begin(), end(), [](const Point& p) {
			return p.valid();
		});
}


template<typename Point, typename Allocator> template<typename Other_point, typename Other_allocator>
point_cloud<Point, Allocator>::
point_cloud(const point_cloud<Other_point, Other_allocator>& pc, bool all_val, std::size_t cap, const Allocator& alloc) :
	point_cloud( (cap > 0 ? cap : pc.size()) , all_val, alloc)
{
	make_sibling(pc, pc.relative_pose());

	// Copy points one-by-one into new buffer.
	// If this is all valid but other is not, take only valid points.
	Point* buf = begin_;
	if(all_val && ! pc.all_valid()) {
		for(const auto& p : pc) if(p.valid()) *(buf++) = Point(p);
	} else {
		for(const auto& p : pc) *(buf++) = Point(p);
	}
	end_ = buf;
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
std::size_t point_cloud<Point, Allocator>::number_of_invalid_points() const {
	return size() - number_of_valid_points();
}

template<typename Point, typename Allocator>
bool point_cloud<Point, Allocator>::contains_invalid_points() const {
	if(all_valid_) return false;
	for(const Point& p : *this) if(! p.valid()) return false;
	return true;
}



template<typename Point, typename Allocator>
void point_cloud<Point, Allocator>::export_with(point_cloud_exporter& exp) const {
	if(all_valid_) {
		exp.write(data(), size());
	} else {
		const Point* segment_start = nullptr;
		for(const Point* p = begin(); p != end(); ++p) {
			bool valid = p->valid();
			if(!valid && segment_start) {
				exp.write(segment_start, p - segment_start);
				segment_start = nullptr;
			} else if(valid && !segment_start) {
				segment_start = p;
			}
		}
		if(segment_start) exp.write(segment_start, end() - segment_start);
	}
	exp.close();
}



template<typename Point, typename Allocator>
const Point& point_cloud<Point, Allocator>::random_point() const {
	std::ptrdiff_t i = random_integer<std::ptrdiff_t>(size() - 1);
	return *(begin_ + i);
}


template<typename Point, typename Allocator>
Point& point_cloud<Point, Allocator>::random_point() {
	std::ptrdiff_t i = random_integer<std::ptrdiff_t>(size() - 1);
	return *(begin_ + i);
}


template<typename Point, typename Allocator>
auto point_cloud<Point, Allocator>::begin_transform(const Eigen::Affine3f& t) const -> transform_iterator {
	return transform_iterator(begin_, t);
}

template<typename Point, typename Allocator>
auto point_cloud<Point, Allocator>::end_transform() const -> transform_iterator {
	return transform_iterator(end_);
}


template<typename Point, typename Allocator>
auto point_cloud<Point, Allocator>::begin_relative_to(const space_object& obj, const Eigen::Affine3f& transform) const -> transform_iterator {
	return transform_iterator(begin_, transform * transformation_from(obj));
}

template<typename Point, typename Allocator>
auto point_cloud<Point, Allocator>::end_relative_to() const -> transform_iterator {
	return transform_iterator(end_);
}



template<typename Point, typename Allocator>
auto point_cloud<Point, Allocator>::begin_absolute(const Eigen::Affine3f& transform) const -> transform_iterator {
	return transform_iterator(begin_, transform * absolute_pose().transformation_to_world());
}

template<typename Point, typename Allocator>
auto point_cloud<Point, Allocator>::end_absolute() const -> transform_iterator {
	return transform_iterator(end_);
}


template<typename Point, typename Allocator>
void point_cloud<Point, Allocator>::set_point_indices() {
	std::ptrdiff_t i = 0;
	for(Point& pt : *this) pt.set_index(i++);
}



template<typename Point, typename Allocator>
void point_cloud<Point, Allocator>::move_center_of_mass_to_origin() {	
	move(-center_of_mass());	
}

}