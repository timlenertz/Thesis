namespace pcf {

template<typename Callback_func, typename Iterator>
void iterate_and_pick_random(Iterator begin, Iterator end, std::size_t total, std::size_t expected, const Callback_func& cb) {	
	random_generator& rng = get_random_generator();
	
	std::size_t left = total;
	std::size_t needed = expected;
	
	for(Iterator it = begin; it != end; ++it) {
		std::uniform_int_distribution<std::size_t> dist(0, left - 1);
		if(dist(rng) < needed) {
			cb(*it);
			--needed;
		}
		--left;
	}
}


template<typename Callback_func, typename Iterator>
inline void iterate_and_pick_random(Iterator begin, Iterator end, std::size_t expected, const Callback_func& cb) {
	iterate_and_pick_random(begin, end, end - begin, expected, cb);
}

}
