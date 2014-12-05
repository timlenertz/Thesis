#include <cmath>

namespace pcf {

template<typename Point, typename Allocator>
auto range_point_cloud<Point, Allocator>::to_angular(image_coordinates c) const -> angular_image_coordinates {
	std::ptrdiff_t dx = std::ptrdiff_t(c[0]) - image_center_[0];
	std::ptrdiff_t dy = std::ptrdiff_t(c[1]) - image_center_[1];
	return {
		angle(angular_resolution_[0] * dx),
		angle(angular_resolution_[1] * dy)
	};
}


template<typename Point, typename Allocator>
auto range_point_cloud<Point, Allocator>::to_image(angular_image_coordinates c) const -> image_coordinates {
	float dx = c[0] / angular_resolution_[0];
	float dy = c[1] / angular_resolution_[1];
	return {
		std::ptrdiff_t(dx) + image_center_[0],
		std::ptrdiff_t(dy) + image_center_[1]
	};
}


template<typename Point, typename Allocator>
auto range_point_cloud<Point, Allocator>::to_image(const spherical_coordinates& s) const -> image_coordinates {
	return to_image({ s.azimuth, s.elevation });
}


template<typename Point, typename Allocator>
std::size_t range_point_cloud<Point, Allocator>::number_of_pixels_for_camera_(angle rx, angle ry, const camera& cam) {
	std::size_t x = std::ceil( cam.field_of_view_x() / rx );
	std::size_t y = std::ceil( cam.field_of_view_y() / ry );
	return x * y;
}


template<typename Point, typename Allocator> template<typename Other_cloud>
void range_point_cloud<Point, Allocator>::project_(const Other_cloud& pc) {
	std::vector<float> r_buffer(width()*height(), INFINITY);
	
	//#pragma omp parallel for shared(r_buffer)
	for(auto it = pc.cbegin(); it < pc.cend(); ++it) {
		const auto& p = *it;
		spherical_coordinates s = camera_.to_spherical(p);
		image_coordinates i = to_image(s);
		if(! in_bounds(i)) continue;
		
		std::ptrdiff_t off = offset_(i);
		float& closest_r = r_buffer[off];
		
		if(s.radius < closest_r) {
			closest_r = s.radius;
			super::at(off) = p;
		}
	}
}


template<typename Point, typename Allocator> template<typename Other_cloud>
range_point_cloud<Point, Allocator>::range_point_cloud(const Other_cloud& pc, const camera& cam, angle rx, angle ry, const Allocator& alloc) :
super(number_of_pixels_for_camera_(rx, ry, cam), false, alloc), camera_(cam) {
	angular_resolution_[0] = rx; angular_resolution_[1] = ry;

	image_size_[0] = std::ceil(cam.field_of_view_x() / angular_resolution_[0]);
	image_size_[1] = std::ceil(cam.field_of_view_y() / angular_resolution_[1]);
	image_center_[0] = std::ptrdiff_t(image_size_[0]) / 2;
	image_center_[1] = std::ptrdiff_t(image_size_[1]) / 2;


	super::resize_(super::capacity());
	super::initialize_();
	project_(pc);
}


template<typename Point, typename Allocator>
range_point_cloud<Point, Allocator>::range_point_cloud(const range_image& ri, const camera& cam, const Allocator& alloc) :
super(ri.number_of_pixels(), false, alloc), camera_(cam) {
	image_size_[0] = ri.width(); image_size_[1] = ri.height();
	angular_resolution_[0] = cam.field_of_view_x() / ri.width(); angular_resolution_[1] = cam.field_of_view_y() / ri.height(); 
	for(std::ptrdiff_t i = 0; i < 2; ++i) image_center_[i] = std::ptrdiff_t(image_size_[i]) / 2;

	super::resize_(super::capacity());
	super::initialize_();

	Point* p = super::begin();
	for(std::ptrdiff_t y = 0; y < height(); ++y) {
		for(std::ptrdiff_t x = 0; x < width(); ++x, ++p) {
			if(! ri.valid(x, y)) continue;
			
			float r = ri.at(x, y);
			angular_image_coordinates a = to_angular({x, y});
			spherical_coordinates s(r, a[0], a[1]);
			
			*p = Point( camera_.point(s) );
		}
	}
}


template<typename Point, typename Allocator>
range_point_cloud<Point, Allocator>::range_point_cloud(range_point_cloud_importer& imp, const Allocator& alloc) :
super(imp.size(), false, alloc), camera_() {
	
}


template<typename Point, typename Allocator> 
inline std::ptrdiff_t range_point_cloud<Point, Allocator>::offset_(image_coordinates c) const {
	return (width() * c[1]) + c[0];
}


template<typename Point, typename Allocator> 
inline bool range_point_cloud<Point, Allocator>::in_bounds(image_coordinates c) const {
	return (c[0] >= 0) && (c[0] < width())
		&& (c[1] >= 0) && (c[1] < height());
}


/*
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
*/

template<typename Point, typename Allocator>
range_image range_point_cloud<Point, Allocator>::to_range_image() {
	range_image ri(width(), height());
	ri.erase();
	
	const Point* p = super::begin_;
	for(std::ptrdiff_t y = 0; y < height(); ++y) {
		for(std::ptrdiff_t x = 0; x < width(); ++x, ++p) {
			if(! p->valid()) continue;
			
			spherical_coordinates s = camera_.to_spherical(*p);
			ri.at(x, y) = s.radius;
		}
	}

	return ri;
}


}