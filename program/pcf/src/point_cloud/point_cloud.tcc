#include <cmath>
#include <memory>
#include <algorithm>

namespace pcf {

template<typename Point, typename Allocator>
void point_cloud<Point, Allocator>::check_correct_alignment_() const {
	if((std::uintptr_t)buffer_ % alignof(Point))
		throw std::runtime_error("Point cloud data not properly aligned.");
}

template<typename Point, typename Allocator>
void point_cloud<Point, Allocator>::resize_(std::size_t new_size) {	
	if(new_size > capacity()) throw std::logic_error("Point cloud new size cannot be greater than capacity.");
	else buffer_end_ = buffer_ + new_size;
}

template<typename Point, typename Allocator>
void point_cloud<Point, Allocator>::initialize_() {	
	for(Point* p = buffer_; p < buffer_end_; ++p) p->invalidate();
}

template<typename Point, typename Allocator>
point_cloud<Point, Allocator>::point_cloud(std::size_t allocate_size, bool all_val, const Allocator& alloc) :
	allocator_(alloc),
	buffer_(allocator_.allocate(allocate_size)),
	buffer_end_(buffer_),
	allocated_size_(allocate_size),
	all_valid_(all_val)
{
	check_correct_alignment_();
}

template<typename Point, typename Allocator>
point_cloud<Point, Allocator>::~point_cloud() {
	if(buffer_) allocator_.deallocate(buffer_, allocated_size_);
}

template<typename Point, typename Allocator>
point_cloud<Point, Allocator>::point_cloud(const point_cloud& pc, bool all_val, const Allocator& alloc) :
point_cloud(pc.capacity(), all_val, alloc) {
	resize_(pc.size());
	std::memcpy((void*)buffer_, (const void*)pc.buffer_, pc.size()*sizeof(Point));
	
	if(all_valid_ && !pc.all_valid_) erase_invalid_points();
}

template<typename Point, typename Allocator>
point_cloud<Point, Allocator>::point_cloud(point_cloud&& pc, bool all_val) :
allocator_( std::move(pc.allocator_) ),
buffer_(pc.buffer_),
buffer_end_(pc.buffer_end_),
allocated_size_(pc.allocated_size_),
all_valid_(all_val) {
	pc.buffer_ = nullptr;
	if(all_valid_ && !pc.all_valid_) erase_invalid_points();
}



template<typename Point, typename Allocator> template<typename Other>
point_cloud<Point, Allocator>::point_cloud(const Other& pc, bool all_val, const Allocator& alloc) :
point_cloud(pc.capacity(), all_val, alloc) {
	resize_(pc.size());
	Point* o = buffer_;
	
	#pragma omp parallel for
	for(typename Other::const_iterator i = pc.cbegin(); i < pc.cend(); ++i) *(o++) = *i;
	
	if(all_valid_ && !pc.all_valid_) erase_invalid_points();
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
	reader.template read<Point>(buffer_, size());
}


template<typename Point, typename Allocator>
std::size_t point_cloud<Point, Allocator>::number_of_valid_points() const {
	if(all_valid_) {
		return size();
	} else {
		std::size_t total = 0;
		#pragma omp parallel for reduction(+:total)
		for(const Point* p = buffer_; p < buffer_end_; ++p) if(p->valid()) ++total;
		return total;
	}
}

template<typename Point, typename Allocator>
bool point_cloud<Point, Allocator>::contains_invalid_points() const {
	if(all_valid_) return false;
		
	bool found_invalid = false;
	#pragma omp parallel for shared(found_invalid)
	for(const Point* p = buffer_; p < buffer_end_; ++p) {
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
	writer.write(buffer_, size());
}


template<typename Point, typename Allocator> template<typename Transformation>
void point_cloud<Point, Allocator>::apply_transformation(const Transformation& t) {
	const Eigen::Affine3f affinet(t);
	#pragma omp parallel for
	for(Point* p = buffer_; p < buffer_end_; ++p) p->apply_transformation(affinet);
}

template<typename Point, typename Allocator>
Eigen::Vector3f point_cloud<Point, Allocator>::center_of_mass() const {
	Eigen::Vector4f sum = Eigen::Vector4f::Zero();
	std::size_t total = size();
	
	if(all_valid_) {
		#pragma omp parallel
		{
			Eigen::Vector4f sum_part = Eigen::Vector4f::Zero();
			#pragma omp for
			for(Point* p = buffer_; p < buffer_end_; ++p) sum_part += p->homogeneous_coordinates;
		
			#pragma omp critical
			{ sum += sum_part; }
		}
	} else {
		#pragma omp parallel
		{
			Eigen::Vector4f sum_part = Eigen::Vector4f::Zero();
			
			total = 0;
			#pragma omp for reduction(+:total)
			for(Point* p = buffer_; p < buffer_end_; ++p) {
				if(! p->valid()) continue;
				sum_part += p->homogeneous_coordinates;
				++total;
			}
		
			#pragma omp critical
			{ sum += sum_part; }
		}
	}
	
	return (sum / total).head(3);
}

template<typename Point, typename Allocator>
cuboid point_cloud<Point, Allocator>::bounding_cuboid(float ep) const {
	const float inf = INFINITY;
	cuboid cub = cuboid::infinite();
	
	#pragma omp parallel
	{
		Eigen::Vector4f mn_part(+inf, +inf, +inf, 0);
		Eigen::Vector4f mx_part(-inf, -inf, -inf, 0);
		
		#pragma omp for
		for(Point* p = buffer_; p < buffer_end_; ++p) {
			if(! p->valid()) continue;
			const Eigen::Vector4f pc = p->homogeneous_coordinates;
			
			mn_part = mn_part.cwiseMin(pc);
			mx_part = mx_part.cwiseMax(pc);
		}
		
		#pragma omp critical
		{
			cub.origin = cub.origin.cwiseMin(mn_part.head(3));
			cub.extremity = cub.extremity.cwiseMax(mx_part.head(3));
		}
	}
	
	cub.extremity += Eigen::Vector3f(ep, ep, ep);
	return cub;
}

template<typename Point, typename Allocator>
template<typename Other_point, typename Distance_func>
const Point& point_cloud<Point, Allocator>::find_closest_point(const Other_point& from, Distance_func dist, const Point* start, const Point* end) const {
	if(! start) start = buffer_;
	if(! end) end = buffer_;

	float minimal_distance = INFINITY;
	const Point* closest_point = nullptr;
	
	#pragma omp parallel
	{
		float min_d = INFINITY;
		const Point* cp = nullptr;

		#pragma omp for
		for(const Point* p = start; p < end; ++p) {
			if(!all_valid_) if(! p->valid()) continue;
			float d = dist(*p, from);								
			if(d < min_d) { min_d = d; cp = p; }
		}

		#pragma omp critical
		if(min_d < minimal_distance) { minimal_distance = min_d; closest_point = cp; }
	}
	
	return *closest_point;
}


template<typename Point, typename Allocator>
void point_cloud<Point, Allocator>::erase_invalid_points() {
	if(! all_valid_) {
		Point* np = buffer_;
		for(Point* p = buffer_; p < buffer_end_; ++p) if(p->valid()) *(np++) = *p;
		buffer_end_ = np;
	}
}


template<typename Point, typename Allocator> template<typename Random_generator>
void point_cloud<Point, Allocator>::downsample_random(float ratio, bool invalidate) {
	if(invalidate && all_valid_) throw std::invalid_argument("Cannot invalidate points in all valid point cloud.");
	
	Random_generator rng;
	Point* np = buffer_;
	
	const std::size_t total = number_of_valid_points();
	const std::size_t expected = ratio * total;
	std::size_t picked = 0;
	std::size_t left = total;
	std::size_t needed = expected;
	
	for(Point* p = buffer_; p < buffer_end_; ++p) {
		if(! all_valid_) if(! p->valid()) continue;
		std::uniform_int_distribution<std::size_t> dist(0, left - 1);
		if(dist(rng) < needed) {
			if(invalidate) p->invalidate();
			else *(np++) = *p;
			--needed;
		}
		--left;
	}
	if(! invalidate) buffer_end_ = np;
}


template<typename Point, typename Allocator> template<typename Compare_func>
void point_cloud<Point, Allocator>::sort_points(Compare_func func, Point* start, Point* end) {
	if(! start) start = buffer_;
	if(! end) end = buffer_;

	std::sort(start, end, func);
}

}