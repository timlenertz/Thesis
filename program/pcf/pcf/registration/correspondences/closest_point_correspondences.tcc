#include <vector>

namespace pcf {


template<typename Cloud_fixed, typename Cloud_loose, typename Selection_func, typename Weight_func>
auto closest_point_correspondences<Cloud_fixed, Cloud_loose, Selection_func, Weight_func>::operator() () -> correspondences_type {
	super::clear();

	#pragma omp parallel
	{
		std::vector<super::correspondence> cors_part;
		
		#pragma omp for
		for(auto it = loose_.cbegin(); it < loose_.cend(); ++it) {
			const auto& lp = *it;
			if(!lp.valid() || !selection_func_(lp)) continue;
			
			const auto& fp = fixed_.find_closest_point(lp);
			if(! fp.value()) continue;
			
			float w = weight_func(fp, lp);
			if(w != 0) cors_part.emplace_back(fp, lp, w);
		}
		
		#pragma critical
		{
			super::insert(cors_part.begin(), cors_part.end());
		}
	}
}


}
