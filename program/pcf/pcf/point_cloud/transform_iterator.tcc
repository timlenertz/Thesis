namespace pcf {

template<typename Point>
void point_transform_iterator<Point>::load_transformed_point_() const {
	transformed_point_ = *point_;
	if(transformed_point_.valid()) transformed_point_.apply_transformation(transformation_);
	should_load_transformed_point_ = false;
}


template<typename Point>
point_transform_iterator<Point>::point_transform_iterator(const Eigen::Affine3f& t, Point* p) :
transformation_(t), point_(p), should_load_transformed_point_(true) {
	load_transformed_point_();
}


template<typename Point>
bool point_transform_iterator<Point>::operator==(const point_transform_iterator& it) const {
	return (point_ == it.point_);
}


template<typename Point>
bool point_transform_iterator<Point>::operator!=(const point_transform_iterator& it) const{
	return (point_ != it.point_);
}


template<typename Point>
const Point& point_transform_iterator<Point>::operator*() const {
	if(should_load_transformed_point_) load_transformed_point_();
	return transformed_point_;
}


template<typename Point>
const Point* point_transform_iterator<Point>::operator->() const {
	if(should_load_transformed_point_) load_transformed_point_();
	return &transformed_point_;
}


template<typename Point>
auto point_transform_iterator<Point>::operator++() -> point_transform_iterator& {
	++point_;
	should_load_transformed_point_ = true;
	return *this;
}


template<typename Point>
auto point_transform_iterator<Point>::operator++(int) -> point_transform_iterator {
	point_transform_iterator old = *this;
	operator++();
	return old;
}

}