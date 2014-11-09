namespace pcf {

template<typename Cloud_fixed, typename Cloud_loose, typename Selection_func, typename Modifier_func>
closest_point_correspondences<Cloud_fixed, Cloud_loose, Selection_func, Modifier_func>::closest_point_correspondences
(const Cloud_fixed& fx, const Cloud_loose& ls, const Selection_func& sel, const Weight_func& mod) :
fixed_(fx), loose_(ls), selection_func_(sel), modifier_func_(mod) { }



template<typename Cloud_fixed, typename Cloud_loose, typename Selection_func, typename Modifier_func>
auto closest_point_correspondences<Cloud_fixed, Cloud_loose, Selection_func, Modifier_func>::operator() () -> correspondences_type {
	correspondences_type cors;

	#pragma omp parallel
	{
		correspondences_type cors_part;
		
		#pragma omp for
		for(auto it = loose_.cbegin(); it < loose_.cend(); ++it) {
			const auto& lp = *it;
			if(!lp.valid() || !selection_func_(lp)) continue;
			
			const auto& fp = fixed_.find_closest_point(lp);
			if(! fp.value()) continue;
			
			correspondences_type::value_type cor(fp, lp);
			bool keep = modifier_func_(cor);
			
			if(keep) cors_part.push_back(cor);
		}
		
		#pragma critical
		{
			cors.insert(cors.end(), cors_part.begin(), cors_part.end());
		}
	}
	
	return cors;
}


}
