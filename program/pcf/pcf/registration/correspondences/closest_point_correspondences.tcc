#include <vector>

namespace pcf {

template<typename Cloud_fixed, typename Cloud_loose, typename Selection_func, typename Weight_func> template<typename Callback_func>
void closest_point_correspondences<Cloud_fixed, Cloud_loose, Selection_func, Weight_func>::operator() (const Callback_func& callback, bool par) {
	#pragma omp parallel for if(par)
	for(auto it = loose_.cbegin(); it < loose_.cend(); ++it) {
		const auto& lp = *it;
		if(!lp.valid() || !selection_func_(lp)) continue;
		
		const auto& fp = fixed_.find_closest_point(lp);
		if(!fp.valid()) continue;
		
		float w = weight_func_(fp, lp);
		if(w != 0) callback( correspondence_type(fp, lp, w) );
	}
}


}
