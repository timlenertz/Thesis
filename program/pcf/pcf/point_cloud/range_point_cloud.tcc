namespace pcf {


template<typename Point, typename Allocator> template<typename Other_cloud>
range_point_cloud<Point, Allocator>::range_point_cloud(const Other_cloud& pc, const camera& cam, const Allocator& alloc) :
super(cam.number_of_pixels(), false, alloc), camera_(cam) {
	super::initialize_();
	for(auto it = pc.cbegin(); it < pc.cend(); ++it) {
		auto c = camera_.project(*it);
		if(in_bounds(c)) at(c) = *it;
	}
}


template<typename Point, typename Allocator> 
inline std::ptrdiff_t range_point_cloud<Point, Allocator>::offset_(image_coordinates c) {
	return c[0] + (camera_.get_image_width() * c[1]);
}


template<typename Point, typename Allocator> 
inline bool range_point_cloud<Point, Allocator>::in_bounds(image_coordinates c) const {
	return (c[0] >= 0) && (c[0] < camera_.get_image_width())
		&& (c[1] >= 0) && (c[1] < camera_.get_image_height());
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
	range_image ri(width_, height_);
	const Point* p = super::begin_;
	for(std::ptrdiff_t y = 0; y < height_; ++y) {
		for(std::ptrdiff_t x = 0; x < width_; ++x) {
			ri.at(x, y) = camera_.depth(*p);
			++p;
		}
	}
	return ri;
}


}