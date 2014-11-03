namespace pcf {
	
inline bounding_box octree_traits::child_box(std::ptrdiff_t i, const bounding_box& cub, const node_attributes& attr, std::ptrdiff_t depth) {
	Eigen::Vector3f c = cub.center();
	bool x = (i % 2);
	bool y = (i % 4 > 1);
	bool z = (i >= 4);
	
	Eigen::Vector3f corigin(
		x ? c[0] : cub.origin[0],
		y ? c[1] : cub.origin[1],
		z ? c[2] : cub.origin[2]
	);
	Eigen::Vector3f cextremity(
		x ? cub.extremity[0] : c[0],
		y ? cub.extremity[1] : c[1],
		z ? cub.extremity[2] : c[2]
	);
	return bounding_box(corigin, cextremity);
}


template<typename Point>
bounding_box octree_traits::root_box(point_cloud_segment<Point> seg) {
	return seg.box(0.1);
}


template<typename Other_point>
std::ptrdiff_t octree_traits::child_containing_point(const Other_point& p, const bounding_box& cub, const node_attributes& attr, std::ptrdiff_t depth) {
	Eigen::Vector3f c = cub.center();
	std::ptrdiff_t i = 0;
	if(p[0] >= c[0]) i += 1;
	if(p[1] >= c[1]) i += 2;
	if(p[2] >= c[2]) i += 4;
	return i;
}


template<typename Point>
std::array<point_cloud_segment<Point>, 8> octree_traits::split_node(point_cloud_segment<Point> seg, const bounding_box& cub, node_attributes& attr, std::ptrdiff_t depth) {
	auto idx = [&cub](const Point& p) {
		return child_containing_point(p, cub, node_attributes(), 0);
	};
	std::array<point_cloud_segment<Point>, 8> child_segs;
	seg.partition_into_segments(idx, 8, child_segs.begin());
	return child_segs;
}



}

