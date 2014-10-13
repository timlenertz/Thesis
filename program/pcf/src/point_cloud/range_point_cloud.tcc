#ifndef PCF_RANGE_POINT_CLOUD_H_
#error Header not included
#endif

namespace pcf {


template<typename Point, typename Allocator> template<typename Transformation>
void range_point_cloud<Point, Allocator>::apply_transformation(const Transformation& t) {
	super::apply_transformation(t);
	projection_matrix_ *= t.inverse();
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
inline auto range_point_cloud<Point, Allocator>::project(const Other_point& p) -> image_coordinates {
	float ignore;
	return project(p, ignore);
}


template<typename Point, typename Allocator> template<typename Other_point>
auto range_point_cloud<Point, Allocator>::project(const Other_point& p, float& depth) -> image_coordinates {
	Eigen::Vector4f proj = projection_matrix_ * p.homogeneous_coordinates;
	proj /= proj[3];
	depth = proj[2];
	return {
		0.5f*(proj[0] + 1.0) * width_,
		0.5f*(proj[1] + 1.0) * height_
	};
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