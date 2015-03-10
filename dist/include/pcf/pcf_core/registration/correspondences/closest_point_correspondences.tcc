#include <vector>
#include <thread>
#include <utility>

#include <iostream>


namespace pcf {


template<typename Cloud_fixed, typename Cloud_loose, typename Selection_func, typename Weight_func> template<typename Receiver>
inline void closest_point_correspondences<Cloud_fixed, Cloud_loose, Selection_func, Weight_func>::process_point_
(const loose_point_type& lp, const loose_point_type& rlp, Receiver& rec) {
	if(!lp.valid() || !selection_func_(lp)) return;

	const auto& fp = fixed_.closest_point(lp, accepting_distance, rejecting_distance);
	if(!fp.valid()) return;

	float w = weight_func_(fp, lp);
	if(w != 0) rec << correspondence_type(fp, lp, rlp, w);
}


template<typename Cloud_fixed, typename Cloud_loose, typename Selection_func, typename Weight_func> template<typename Receiver>
void closest_point_correspondences<Cloud_fixed, Cloud_loose, Selection_func, Weight_func>::operator()(Receiver& rec, const Eigen::Affine3f& transformation) {
#ifdef _OPENMP

	#pragma omp parallel
	{
		Receiver rec_part;
		
		#pragma omp for
		for(auto it = loose_.begin_relative_to(fixed_, transformation.inverse()); it < loose_.end_relative_to(); ++it)
			process_point_(*it, it.real_point(), rec_part);
		
		#pragma omp critical
		{
			rec << rec_part;
		}
	}
	
#else

	// Parallelize this even w/o OpenMP.
	
	const std::size_t num_threads = std::thread::hardware_concurrency();
	if(num_threads <= 1) {
		for(auto it = loose_.begin_relative_to(fixed_); it != loose_.end_relative_to(); ++it)
			process_point_(*it, it.real_point(), rec);
		return;
	}

	auto worker = [&](Receiver& rec_part, std::ptrdiff_t from, std::ptrdiff_t to) {
		auto init_it = loose_.begin_relative_to(fixed_, transformation.inverse());
		auto end_it = init_it + to;
		for(auto it = init_it + from; it != end_it; ++it)
			process_point_(*it, it.real_point(), rec_part);
	};
	
	std::vector<Receiver> rec_parts(num_threads);
	std::vector<std::thread> threads;
		
	const std::size_t total = loose_.size();
	const std::size_t per_thread = total / num_threads;
	std::ptrdiff_t offset = 0;
	for(std::size_t i = 0; i < num_threads-1; ++i) {
		std::thread th(worker, std::ref(rec_parts[i]), offset, offset + per_thread);
		threads.push_back(std::move(th));
		offset += per_thread;
	}
	
	std::thread th(worker, std::ref(rec_parts.back()), offset, total);
	threads.push_back(std::move(th));

	for(auto& th : threads) th.join();
	for(auto& rec_part : rec_parts) rec << rec_part;

#endif
}

}