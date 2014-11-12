namespace pcf {

template<typename Fixed, typename Loose>
void registration_correspondences<Fixed, Loose>::clear() {
	cors_.clear();
}	


template<typename Fixed, typename Loose>
auto registration_correspondences<Fixed, Loose>::insert(const correspondence& cor) -> correspondence& {
	cors_.push_back(cor);
	return cors_.back();
}


template<typename Fixed, typename Loose> template<typename Iterator>
void registration_correspondences<Fixed, Loose>::insert(Iterator b, Iterator e) {
	cors_.insert(cors_.end(), b, e);
}


template<typename Fixed, typename Loose> 
float registration_correspondences<Fixed, Loose>::mean_squared_error() const {
	float err = 0;
	#pragma omp parallel for reduction(+:err)
	for(auto it = begin(); it < end(); ++it) {
		err += euclidian_distance_sq(it->fixed(), it->loose());
	}
	return err / size();
}

}