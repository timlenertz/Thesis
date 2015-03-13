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
	// transformation is current estimate of Loose's transformation relative to Fixed
	// i.e. Need to apply the inverse transformation to Loose's points when looking for correspondences,
	//      with the remaining transformation.

	auto begin_it = loose_.begin_relative_to(fixed_, transformation);
	auto end_it = loose_.end_relative_to();

#ifdef _OPENMP

	#pragma omp parallel
	{
		Receiver rec_part;
		
		#pragma omp for
		for(auto it = begin_it; it < end_it; ++it)
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
		auto part_end_it = begin_it + to;
		auto part_begin_it = begin_it + from;
		for(auto it = part_begin_it; it != part_end_it; ++it)
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
