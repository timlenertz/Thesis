#include <cmath>
#include <memory>
#include <algorithm>
#include "../util/alignment.h"
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
point_cloud<Point, Allocator>::~point_cloud() {
	if(begin_) allocator_.deallocate(begin_, allocated_size_);
}

template<typename Point, typename Allocator>
point_cloud<Point, Allocator>::point_cloud(const point_cloud& pc, bool all_val) :
point_cloud(pc.capacity(), all_val) {
	resize_(pc.size());
	std::memcpy((void*)begin_, (const void*)pc.begin_, pc.size()*sizeof(Point));
	
	if(all_valid_ && !pc.all_valid()) erase_invalid_points();
}


template<typename Point, typename Allocator>
point_cloud<Point, Allocator>::point_cloud(point_cloud&& pc, bool all_val) :
allocator_( std::move(pc.allocator_) ),
allocated_size_(pc.allocated_size_),
all_valid_(all_val),
begin_( pc.begin_ ),
end_( pc.end_ ) {
	pc.begin_ = nullptr;
	if(all_valid_ && !pc.all_valid_) erase_invalid_points();
}


template<typename Point, typename Allocator> template<typename Other_point, typename Other_allocator>
point_cloud<Point, Allocator>::point_cloud(const point_cloud<Other_point, Other_allocator>& pc, bool all_val, const Allocator& alloc) :
point_cloud(pc.capacity(), all_val, alloc) {
	resize_(pc.size());
	
	auto o = begin();
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
	reader.template read<Point>(data(), size());
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


template<typename Point, typename Allocator> template<typename Transformation>
void point_cloud<Point, Allocator>::apply_transformation(const Transformation& t) {
	const Eigen::Affine3f affinet(t);
	
	#pragma omp parallel for
	for(auto p = begin(); p < end(); ++p) p->apply_transformation(affinet);
}


template<typename Point, typename Allocator>
void point_cloud<Point, Allocator>::erase_invalid_points() {
	if(all_valid_) return;
	
	end_ = std::partition(begin(), end(), [](const Point& p) {
		return p.valid();
	});
}

template<typename Point, typename Allocator> template<typename Random_generator>
void point_cloud<Point, Allocator>::downsample_random(float ratio, bool invalidate) {
	if(invalidate && all_valid_)
		throw std::invalid_argument("Cannot invalidate points in all-valid point cloud.");
	
	Random_generator rng;
	Point* np = begin_;
	
	const std::size_t total = number_of_valid_points();
	const std::size_t expected = ratio * total;
	std::size_t left = total;
	std::size_t needed = expected;
	
	for(Point* p = begin_; p < end_; ++p) {
		if(! all_valid_) if(! p->valid()) continue;
		std::uniform_int_distribution<std::size_t> dist(0, left - 1);
		if(dist(rng) < needed) {
			if(invalidate) p->invalidate();
			else *(np++) = *p;
			--needed;
		}
		--left;
	}
	if(! invalidate) end_ = np;
}



template<typename Point, typename Allocator>
void point_cloud<Point, Allocator>::downsample_grid(float cell_sz, bool move, bool invalidate) {
	if(invalidate && all_valid_)
		throw std::invalid_argument("Cannot invalidate points in all-valid point cloud.");
	
	bounding_box b = box();
	std::size_t number_of_cells[3];
	std::size_t total_number_of_cells = 1;
	for(std::ptrdiff_t i = 0; i < 3; ++i) {
		number_of_cells[i] = std::ceil(b.side_lengths()[i] / cell_sz);
		total_number_of_cells *= number_of_cells[i];
	}
	
	std::vector<Point*> candidate(total_number_of_cells, nullptr);
	
	auto point_cell_index = [&number_of_cells, &b, &cell_sz](const Point& pt) -> std::ptrdiff_t {
		Eigen::Vector3f p = pt - b.origin;
		std::ptrdiff_t c[3];
		for(std::ptrdiff_t i = 0; i < 3; ++i) c[i] = std::floor(p[i] / cell_sz);
		return
			c[0] * number_of_cells[1] * number_of_cells[2]
			+ c[1] * number_of_cells[2]
			+ c[2];
	}
	
	auto cell_center = [](std::ptrdiff_t idx) -> Eigen::Vector3f {
		
	};
	
	for(Point* p = begin_; p < end_; ++p) {
		auto& cand = candidate[ point_cell_index(p) ];
		if(cand == nullptr) {
			cand = p;
		} else {
			
		}
	}
}



template<typename Point, typename Allocator> template<typename Other_point>
const Point& point_cloud<Point, Allocator>::
closest_point(const Other_point& query, float accepting_distance, float rejecting_distance) const {
	auto it = find_closest_point(query, cbegin(), cend(), accepting_distance);
	if(it == cend()) return invalid_point_();
	else return *it;
}



}