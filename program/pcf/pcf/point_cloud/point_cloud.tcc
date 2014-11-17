#include <cmath>
#include <memory>
#include <algorithm>
#include "../util/alignment.h"
#include "../util/multi_dimensional_dynarray.h"
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


template<typename Point, typename Allocator>
void point_cloud<Point, Allocator>::apply_transformation(const Eigen::Affine3f& t) {	
	#pragma omp parallel for
	for(auto p = begin(); p < end(); ++p) p->apply_transformation(t);
}


template<typename Point, typename Allocator> template<typename Callback>
void point_cloud<Point, Allocator>::with_transformation(const Eigen::Affine3f& t, const Callback& callback) {
	apply_transformation(t);
	callback(*this);
	apply_transformation(t.inverse());	
}


template<typename Point, typename Allocator> template<typename Callback>
void point_cloud<Point, Allocator>::with_transformation(const Eigen::Affine3f& t, const Callback& callback) const {
	const_cast<point_cloud*>(this)->apply_transformation(t);
	callback(*this);
	const_cast<point_cloud*>(this)->apply_transformation(t.inverse());	
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
void point_cloud<Point, Allocator>::downsample_grid(float cell_len, bool move, bool invalidate) {
	if(invalidate && all_valid_)
		throw std::invalid_argument("Cannot invalidate points in all-valid point cloud.");
	
	// Create 3D array storing closest point to center of each cell
	// or nullptr if no point in that cell
	bounding_box b = box();
	std::array<std::size_t, 3> sz;
	for(std::ptrdiff_t i = 0; i < 3; ++i) sz[i] = std::ceil(b.side_lengths()[i] / cell_len);
	
	dynarray3<Point*> cells(sz, nullptr);
	using cell_index = typename dynarray3<Point*>::multi_index;
	
	auto cell_center = [&b, &cell_len](const cell_index& mi) -> Eigen::Vector3f {
		Eigen::Vector3f center;
		for(auto i = 0; i < 3; ++i) center[i] = mi[i] * cell_len;
		return (center + b.origin);
	};
	
	// For each point: Retain closest point to center of each cell
	#pragma omp parallel for
	for(Point* p = begin_; p < end_; ++p) {
		cell_index mi;
		for(auto i = 0; i < 3; ++i) mi[i] = ((*p)[i] - b.origin[i]) / cell_len;
	
		auto& cand = cells[mi];
		if(cand == nullptr) {
			cand = p;
		} else {
			Eigen::Vector3f center = cell_center(mi);
		
			float old_d = distance_sq(*cand, center);
			float new_d = distance_sq(*p, center);
			
			if(new_d < old_d) cand = p;
		}
	}


	// If move: Move points retained for cells to respective cell centers
	if(move) for(auto cit = cells.begin(); cit != cells.end(); ++cit) {
		Point* p = *cit;
		if(p) *p = cell_center(cit.index());
	}
	
	
	// Invalidate all points, and then revalidate those retained for cells
	#pragma omp parallel for
	for(Point* p = begin_; p < end_; ++p) p->invalidate();

	#pragma omp parallel for
	for(auto it = cells.begin_raw(); it < cells.end_raw(); ++it)
		if(*it) (*it)->revalidate();


	// If not in invalidate mode: Need to erase invalid from pc
	if(! invalidate) erase_invalid_points();
}



template<typename Point, typename Allocator> template<typename Other_point>
const Point& point_cloud<Point, Allocator>::
closest_point(const Other_point& query, float accepting_distance, float rejecting_distance) const {
	auto it = find_closest_point(query, cbegin(), cend(), accepting_distance);
	if(it == cend()) return invalid_point_();
	else return *it;
}



}