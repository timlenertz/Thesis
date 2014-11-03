#include <cmath>
#include <memory>
#include <algorithm>
#include "../util/alignment.h"

namespace pcf {

template<typename Point, typename Allocator>
void point_cloud<Point, Allocator>::check_correct_alignment_() const {
	if((std::uintptr_t)super::begin_ % ALIGNOF(Point))
		throw std::runtime_error("Point cloud data not properly aligned.");
}

template<typename Point, typename Allocator>
void point_cloud<Point, Allocator>::resize_(std::size_t new_size) {	
	if(new_size > capacity()) throw std::logic_error("Point cloud new size cannot be greater than capacity.");
	else super::end_ = super::begin_ + new_size;
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
	all_valid_(all_val)
{
	super::begin_ = allocator_.allocate(allocate_size);
	super::end_ = super::begin_;
	check_correct_alignment_();
}



template<typename Point, typename Allocator>
point_cloud<Point, Allocator>::~point_cloud() {
	if(super::begin_) allocator_.deallocate(super::begin_, allocated_size_);
}

template<typename Point, typename Allocator>
point_cloud<Point, Allocator>::point_cloud(const point_cloud& pc, bool all_val) :
point_cloud(pc.capacity(), all_val) {
	resize_(pc.size());
	std::memcpy((void*)super::begin_, (const void*)pc.begin_, pc.size()*sizeof(Point));
	
	if(all_valid_ && !pc.all_valid()) erase_invalid_points();
}


template<typename Point, typename Allocator>
point_cloud<Point, Allocator>::point_cloud(point_cloud&& pc, bool all_val) :
super(pc),
allocator_( std::move(pc.allocator_) ),
allocated_size_(pc.allocated_size_),
all_valid_(all_val) {
	pc.begin_ = nullptr;
	if(all_valid_ && !pc.all_valid_) erase_invalid_points();
}


template<typename Point, typename Allocator> template<typename Other_point, typename Other_allocator>
point_cloud<Point, Allocator>::point_cloud(const point_cloud<Other_point, Other_allocator>& pc, bool all_val, const Allocator& alloc) :
point_cloud(pc.capacity(), all_val, alloc) {
	resize_(pc.size());
	
	auto o = super::begin();
	for(auto i = pc.cbegin(); i < pc.cend(); ++i) *(o++) = *i;
	
	if(all_valid_ && !pc.all_valid()) erase_invalid_points();
}


template<typename Point, typename Allocator> template<typename Reader>
auto point_cloud<Point, Allocator>::create_from_reader(Reader& reader, bool all_val) -> point_cloud {
	point_cloud pc(reader.size(), all_val);
	pc.read(reader);
	return pc;
}

template<typename Point, typename Allocator> template<typename Reader> 
void point_cloud<Point, Allocator>::read(Reader& reader) {
	resize_(reader.size());
	reader.template read<Point>(super::data(), size());
}


template<typename Point, typename Allocator>
std::size_t point_cloud<Point, Allocator>::number_of_valid_points() const {
	if(all_valid_) {
		return size();
	} else {
		std::size_t total = 0;
		#pragma omp parallel for reduction(+:total)
		for(auto p = super::begin(); p < super::end(); ++p) if(p->valid()) ++total;
		return total;
	}
}

template<typename Point, typename Allocator>
bool point_cloud<Point, Allocator>::contains_invalid_points() const {
	if(all_valid_) return false;
		
	bool found_invalid = false;
	#pragma omp parallel for shared(found_invalid)
	for(auto p = super::begin(); p < super::end(); ++p) {
		#ifdef _OPENMP
		if(found_invalid) continue;			
		else if(! p->valid()) found_invalid = true;
		#else
		if(! p->valid()) {
			found_invalid = true;
			break;
		}
		#endif
	}
	return found_invalid;
}


template<typename Point, typename Allocator> template<typename Writer>
void point_cloud<Point, Allocator>::write(Writer& writer) const {
	writer.write(super::data(), size());
}


template<typename Point, typename Allocator> template<typename Transformation>
void point_cloud<Point, Allocator>::apply_transformation(const Transformation& t) {
	const Eigen::Affine3f affinet(t);
	
	#pragma omp parallel for
	for(auto p = super::begin(); p < super::end(); ++p) p->apply_transformation(affinet);
}


template<typename Point, typename Allocator>
void point_cloud<Point, Allocator>::erase_invalid_points() {
	if(all_valid_) return;
	
	super::end_ = std::partition(super::begin(), super::end(), [](const Point& p) {
		return p.valid();
	});
}

template<typename Point, typename Allocator> template<typename Random_generator>
void point_cloud<Point, Allocator>::downsample_random(float ratio, bool invalidate) {
	if(invalidate && all_valid_) throw std::invalid_argument("Cannot invalidate points in all valid point cloud.");
	
	Random_generator rng;
	Point* np = super::begin_;
	
	const std::size_t total = number_of_valid_points();
	const std::size_t expected = ratio * total;
	std::size_t picked = 0;
	std::size_t left = total;
	std::size_t needed = expected;
	
	for(Point* p = super::begin_; p < super::end_; ++p) {
		if(! all_valid_) if(! p->valid()) continue;
		std::uniform_int_distribution<std::size_t> dist(0, left - 1);
		if(dist(rng) < needed) {
			if(invalidate) p->invalidate();
			else *(np++) = *p;
			--needed;
		}
		--left;
	}
	if(! invalidate) super::end_ = np;
}


}