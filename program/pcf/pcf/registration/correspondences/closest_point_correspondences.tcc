#include <vector>

namespace pcf {


template<typename Cloud_fixed, typename Cloud_loose, typename Selection_func, typename Weight_func> template<typename Receiver>
void closest_point_correspondences<Cloud_fixed, Cloud_loose, Selection_func, Weight_func>::operator()(Receiver& rec) {
	#pragma omp parallel
	{
		Receiver rec_part;
		
		#pragma omp for
		for(auto it = loose_.cbegin(); it < loose_.cend(); ++it) {
			const auto& lp = *it;
			if(!lp.valid() || !selection_func_(lp)) continue;
		
			const auto& fp = fixed_.closest_point(lp);
			if(!fp.valid()) continue;
		
			float w = weight_func_(fp, lp);
			if(w != 0) rec_part << correspondence_type(fp, lp, w);
		}
		
		#pragma omp critical
		{
			rec << rec_part;
		}
	}
}

}
