namespace pcf {

template<typename Cloud_fixed, typename Cloud_loose, typename Correspondences, typename Transformation_estimation>
void iterative_correspondences_registration<Cloud_fixed, Cloud_loose, Correspondences, Transformation_estimation>::initialize_() {
	correspondences_();
	error_ = correspondences_.mean_squared_error();
}


template<typename Cloud_fixed, typename Cloud_loose, typename Correspondences, typename Transformation_estimation>
void iterative_correspondences_registration<Cloud_fixed, Cloud_loose, Correspondences, Transformation_estimation>::iteration() {
	correspondences_();
	transformation_ = transformation_estimation_();
	loose_.apply_transformation(transformation_);
	error_ = correspondences_.mean_squared_error();
}


template<typename Cloud_fixed, typename Cloud_loose, typename Correspondences, typename Transformation_estimation>
void iterative_correspondences_registration<Cloud_fixed, Cloud_loose, Correspondences, Transformation_estimation>::run() {
	std::size_t i = 0;
	while(i++ < maximal_iterations_ && error_ >= minimal_error_) iteration();
}

}
