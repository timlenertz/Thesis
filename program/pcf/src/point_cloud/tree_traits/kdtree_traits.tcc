namespace pcf {
	
inline cuboid kdtree_traits::child_cuboid(std::ptrdiff_t i, const cuboid& cub, const node_attributes& attr, std::ptrdiff_t depth) {
	std::ptrdiff_t o = depth % 3;
	cuboid c_cub = cub;
	if(i) c_cub.origin[o] = attr.split_plane;
	else c_cub.extremity[o] = attr.split_plane;
	return c_cub;
}


template<typename Point>
cuboid kdtree_traits::root_cuboid(point_cloud_segment<Point> seg) {
	return seg.bounding_cuboid(0.1);
}


template<typename Other_point>
std::ptrdiff_t kdtree_traits::child_for_point(const Other_point& p, const cuboid& cub, const node_attributes& attr, std::ptrdiff_t depth) {
	std::ptrdiff_t o = depth % 3;
	return (p[o] < attr.split_plane ? 0 : 1);
}


template<typename Point>
std::array<point_cloud_segment<Point>, 2> kdtree_traits::split_node(point_cloud_segment<Point> seg, const cuboid& cub, node_attributes& attr, std::ptrdiff_t depth) {
	std::ptrdiff_t o = depth % 3;

	auto median = seg.begin() + seg.size()/2;
	std::nth_element(seg.begin(), median, seg.end(), [attr, o](const Point& a, const Point& b) {
		return (a[o] < b[o]);
	});

	attr.split_plane = (*median)[o];

	return {
		point_cloud_segment<Point>(seg.begin(), median),
		point_cloud_segment<Point>(median, seg.end())
	};
}



}

