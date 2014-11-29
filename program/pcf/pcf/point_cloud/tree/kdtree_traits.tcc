namespace pcf {
	
inline bounding_box kdtree_traits::
child_box(std::ptrdiff_t i, const bounding_box& cub, const node_attributes& attr, std::ptrdiff_t depth) {
	std::ptrdiff_t o = depth % 3;
	bounding_box c_cub = cub;
	if(i) c_cub.origin[o] = attr.split_plane;
	else c_cub.extremity[o] = attr.split_plane;
	return c_cub;
}


template<typename Point>
bounding_box kdtree_traits::
root_box(point_cloud_segment<Point> seg) {
	return seg.box(0.1);
}


template<typename Other_point>
std::ptrdiff_t kdtree_traits::
child_containing_point(const Other_point& p, const bounding_box& cub, const node_attributes& attr, std::ptrdiff_t depth) {
	std::ptrdiff_t o = depth % 3;
	return (p[o] < attr.split_plane ? 0 : 1);
}


template<typename Other_point>
std::ptrdiff_t kdtree_traits::
child_box_closest_to_point(const Other_point& p, const bounding_box& cub, const node_attributes& attr, std::ptrdiff_t depth) {
	std::ptrdiff_t o = depth % 3;
	return (p[o] < attr.split_plane ? 0 : 1);
}


template<typename Other_point>
std::ptrdiff_t kdtree_traits::
child_box_closer_to_point(const Other_point& p, std::ptrdiff_t a, std::ptrdiff_t b, const bounding_box& cub, const node_attributes& attr, std::ptrdiff_t depth) {
	return child_containing_point(p, cub, attr, depth);
}


template<typename Point>
std::array<point_cloud_segment<Point>, 2> kdtree_traits::
split_node(point_cloud_segment<Point> seg, const bounding_box& cub, node_attributes& attr, std::ptrdiff_t depth) {
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


template<typename Point>
octree_traits::node_attributes kdtree_traits::initialize_node_attributes
(point_cloud_segment<Point> seg, const bounding_box& cub, std::ptrdiff_t depth) {
	return node_attributes();
}


}

