namespace pcf {

template<typename Point, typename Iterator>
unorganized_point_cloud<Point> merge_point_clouds(Iterator begin, Iterator end) {
	using point_cloud_type = typename Iterator::value_type;

	std::size_t total_size = 0;
	for(const auto& pc_it = begin; pc_it != end; ++pc_it)
		total_size += pc_it->number_of_valid_points();
	
	const point_cloud_type& first_pc = *begin;
	
	unorganized_point_cloud<Point> pc(total_size);
	auto it = pc.begin();
	for(const auto& pc_it = begin; pc_it != end; ++pc_it) {
		const point_cloud_type& pc = *pc_it;
		it = pc.insert(it, pc.begin_relative_to(first_pc), pc.end_relative_to());
	}
}


template<typename Point, typename Other_point>
unorganized_point_cloud<Point> operator+(const point_cloud<Point>& pc1, const point_cloud<Other_point>& pc2) {
	unorganized_point_cloud<Point> pc(pc1.number_of_valid_points() + pc2.number_of_valid_points());
	auto it = pc.insert(pc.begin(), pc1.cbegin(), pc1.cend());
	pc.insert(it, pc2.begin_relative_to(pc1), pc2.end_relative_to());
	return pc;
}

}