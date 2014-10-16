namespace pcf {

template<typename Cloud_fixed, typename Cloud_loose>
void closest_point_correspondences<Cloud_fixed, Cloud_loose>::compute() {
	super::cors_.clear();
	
	#pragma omp parallel
	{
		std::vector<correspondence_type> cors_part;
		
		#pragma omp for
		for(auto it = super::loose_cloud_.cbegin(); it < super::loose_cloud_.cend(); ++it) {
			const auto& pl = *it;
			if(! pl.valid()) continue;
			const auto& pf = super::fixed_cloud_.find_closest_point(pl, euclidian_distance_sq);
			if(pf.valid()) cors_part.emplace_back(pf, pl);
		}
		
		#pragma omp critical
		{
			super::cors_.insert(super::cors_.end(), cors_part.begin(), cors_part.end());
		}
	}
}

}
