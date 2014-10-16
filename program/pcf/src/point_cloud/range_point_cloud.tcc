#ifndef PCF_RANGE_POINT_CLOUD_H_
#error Header not included
#endif

namespace pcf {

template<typename Point, typename Allocator>
range_point_cloud<Point, Allocator>::range_point_cloud(std::size_t w, std::size_t h, const Allocator& alloc) :
super(w * h, false, true, alloc), width_(w), height_(h) {
	super::resize_(w * h);
	super::initialize_();
}


template<typename Point, typename Allocator> template<typename Transformation>
void range_point_cloud<Point, Allocator>::apply_transformation(const Transformation& t) {
	super::apply_transformation(t);
	projection_matrix_ = projection_matrix_ * t.inverse();
}

template<typename Point, typename Allocator>	
void range_point_cloud<Point, Allocator>::erase() {
	#pragma omp parallel for
	for(Point* p = super::buffer_; p < super::buffer_end_; ++p) p->invalidate();
}


template<typename Point, typename Allocator>
float range_point_cloud<Point, Allocator>::depth(const Point& p) const {
	if(p.valid()) {
		Eigen::Vector4f proj = projection_matrix_ * p.homogeneous_coordinates;
		return proj[2] / proj[3];
	} else {
		return 0;
	}
}


template<typename Point, typename Allocator> template<typename Other_point>
inline auto range_point_cloud<Point, Allocator>::project(const Other_point& p) const -> image_coordinates {
	float ignore;
	return project(p, ignore);
}


template<typename Point, typename Allocator> template<typename Other_point>
auto range_point_cloud<Point, Allocator>::project(const Other_point& p, float& depth) const -> image_coordinates {
	Eigen::Vector4f proj = projection_matrix_ * p.homogeneous_coordinates;
	proj /= proj[3];
	depth = proj[2];
	return {
		std::size_t( 0.5f*(proj[0] + 1.0) ) * width_,
		std::size_t( 0.5f*(proj[1] + 1.0) ) * height_
	};
}


template<typename Point, typename Allocator>
template<typename Other_point, typename Distance_func>
const Point& range_point_cloud<Point, Allocator>::find_closest_point(const Other_point& from, Distance_func dist, unsigned neightborhood_radius) const {	
	static Point invalid = Point();

	//float minimal_distance = std::numeric_limits<float>::infinity();
	//Point* closest_point = nullptr;

	float min_d = std::numeric_limits<float>::infinity();
	const Point* cp = &invalid;

	image_coordinates center = project(from);
	
	std::ptrdiff_t x_min = center.x - neightborhood_radius; if(x_min < 0) x_min = 0;
	std::ptrdiff_t x_max = center.x + neightborhood_radius; if(x_max >= width_) x_max = width_ - 1;
	std::ptrdiff_t y_min = center.y - neightborhood_radius; if(y_min < 0) y_min = 0;
	std::ptrdiff_t y_max = center.y + neightborhood_radius; if(y_max >= height_) y_max = height_ - 1;
	
	for(std::ptrdiff_t y = y_min; y <= y_max; ++y) {
		for(std::ptrdiff_t x = x_min; x <= x_max; ++x) {
			const Point& p = at(x, y);
			if(p.valid()) {
				float d = dist(p, from);								
				if(d < min_d) { min_d = d; cp = &p; }
			}
		}
	}
	
	return *cp;
}


template<typename Point, typename Allocator> template<typename Other_cloud>
void range_point_cloud<Point, Allocator>::project_point_cloud(const Other_cloud& pc, const Eigen::Projective3f& projm) {
	projection_matrix_ = projm;
	std::vector<float> z_buffer(width_ * height_, std::numeric_limits<float>::infinity());

	#pragma omp parallel for
	for(auto p = pc.cbegin(); p < pc.cend(); ++p) {
		Eigen::Vector4f proj = projm * p->homogeneous_coordinates;
		proj /= proj[3];
	
		float z = proj[2];
		if(z < 0.0) continue;

		std::ptrdiff_t x = 0.5f*(proj[0] + 1.0) * width_;
		if(x < 0 || x >= (std::ptrdiff_t)width_) continue;
	
		std::ptrdiff_t y = 0.5f*(proj[1] + 1.0) * height_;
		if(y < 0 || y >= (std::ptrdiff_t)height_) continue;

		std::ptrdiff_t off = offset_(x, y);
	
		float old_z;
		#pragma omp atomic read
		old_z = z_buffer[off];
	
		if(z < old_z) {			
			#pragma omp critical
			{ z_buffer[off] = z; super::buffer_[off] = *p; }
		}
	}
}

template<typename Point, typename Allocator>
range_image range_point_cloud<Point, Allocator>::to_range_image() {
	range_image ri(width_, height_);
	const Point* p = super::buffer_;
	for(std::ptrdiff_t y = 0; y < height_; ++y) {
		for(std::ptrdiff_t x = 0; x < width_; ++x) {
			ri.at(x, y) = depth(*p);
			++p;
		}
	}
	return ri;
}


}