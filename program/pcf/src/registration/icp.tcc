namespace pcf {

template<typename Cloud_fixed, typename Cloud_loose>
void icp<Cloud_fixed, Cloud_loose>::match_closest_points_() {
	correspondence_.erase();
	for(std::ptrdiff_t i = 0; i <= loose_cloud_.size(); ++i) {
		correspondence_.add(fixed_cloud_[i], loose_cloud_.find_closest_point(fixed_cloud_[i], euclidian_distance_sq));
	}
}


template<typename Cloud_fixed, typename Cloud_loose>
void icp<Cloud_fixed, Cloud_loose>::apply_iteration_transformation_() {
	Eigen::Affine3f trans = correspondence_.estimate_transformation_svd();
	loose_cloud_.apply_transformation(trans.inverse());
}


template<typename Cloud_fixed, typename Cloud_loose>
void icp<Cloud_fixed, Cloud_loose>::operator() () {
	for(std::ptrdiff_t i = 0; i < 20; ++i) {
		match_closest_points_();
		apply_iteration_transformation_();
		std::cout << i << ": " << correspondence_.size() << " -> " << correspondence_.mean_squared_error() << std::endl;
	}
}


}