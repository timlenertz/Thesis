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
all_valid_(all_val) {
	check_correct_alignment_();
}

template<typename Point, typename Allocator>
point_cloud<Point, Allocator>::~point_cloud() {
	allocator_.deallocate(buffer_, allocated_size_);
}

template<typename Point, typename Allocator>
point_cloud<Point, Allocator>::point_cloud(const point_cloud& pc, const Allocator& alloc) :
point_cloud(pc.capacity(), pc.all_valid(), alloc) {
	resize_(pc.size());
	std::memcpy((void*)buffer_, (const void*)pc.buffer_, pc.size()*sizeof(Point));
}

template<typename Point, typename Allocator> template<typename Other_alloc>
point_cloud<Point, Allocator>::point_cloud(const point_cloud<Point, Other_alloc>& pc, const Allocator& alloc) :
point_cloud(pc.capacity(), pc.all_valid(), alloc) {
	resize(pc.size());
	std::memcpy((void*)buffer_, (const void*)pc.buffer_, pc.size()*sizeof(Point));
}

template<typename Point, typename Allocator> template<typename Other>
point_cloud<Point, Allocator>::point_cloud(const Other& pc, const Allocator& alloc) :
point_cloud(pc.capacity(), pc.all_valid(), alloc) {
	resize_(pc.size());
	Point* o = buffer_;
	
	#pragma omp parallel for
	for(typename Other::const_iterator i = pc.cbegin(); i < pc.cend(); ++i) *(o++) = *i;
}

template<typename Point, typename Allocator> template<typename Reader>
auto point_cloud<Point, Allocator>::create_from_reader(Reader& reader) -> point_cloud {
	point_cloud pc(reader.size(), true);
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
template<typename Other_point, typename Distance_func>
const Point& point_cloud<Point, Allocator>::find_closest_point(const Other_point& from, Distance_func dist) const {
	float minimal_distance = std::numeric_limits<float>::infinity();
	Point* closest_point = nullptr;
	
	#pragma omp parallel
	{
		float min_d = std::numeric_limits<float>::infinity();
		Point* cp = nullptr;

		#pragma omp for
		for(Point* p = buffer_; p < buffer_end_; ++p) {
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


}