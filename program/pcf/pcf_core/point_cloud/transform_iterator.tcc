namespace pcf {

template<typename Point>
void point_transform_iterator<Point>::load_transformed_point_() const {
	transformed_point_ = *point_;
	if(transformed_point_.valid()) {
		transformed_point_.apply_transformation(transformation_);
		if(transformed_point_.has_normal()) {
			Eigen::Vector3f n = transformed_point_.get_normal();
			Eigen::Vector4f hn(n[0], n[1], n[2], 0.0);
			hn = transformation_ * hn;
			transformed_point_.set_normal(hn.head(3));
		}
	}
	should_load_transformed_point_ = false;
}


template<typename Point>
point_transform_iterator<Point>::point_transform_iterator(Point* p, const Eigen::Affine3f& t) :
transformation_(t), point_(p), should_load_transformed_point_(true) {
	// defer loading transformed point to operator*
	// it may be invalid (end iterator)
}


template<typename Point>
bool point_transform_iterator<Point>::operator==(const point_transform_iterator& it) const {
	return (point_ == it.point_);
}

template<typename Point>
bool point_transform_iterator<Point>::operator!=(const point_transform_iterator& it) const {
	return (point_ != it.point_);
}

template<typename Point>
bool point_transform_iterator<Point>::operator<(const point_transform_iterator& it) const {
	return (point_ < it.point_);
}

template<typename Point>
bool point_transform_iterator<Point>::operator<=(const point_transform_iterator& it) const {
	return (point_ <= it.point_);
}

template<typename Point>
bool point_transform_iterator<Point>::operator>(const point_transform_iterator& it) const {
	return (point_ > it.point_);
}

template<typename Point>
bool point_transform_iterator<Point>::operator>=(const point_transform_iterator& it) const {
	return (point_ >= it.point_);
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
auto point_transform_iterator<Point>::operator--() -> point_transform_iterator& {
	--point_;
	should_load_transformed_point_ = true;
	return *this;
}


template<typename Point>
auto point_transform_iterator<Point>::operator++(int) -> point_transform_iterator {
	point_transform_iterator old = *this;
	operator++();
	return old;
}


template<typename Point>
auto point_transform_iterator<Point>::operator--(int) -> point_transform_iterator {
	point_transform_iterator old = *this;
	operator--();
	return old;
}


template<typename Point>
auto point_transform_iterator<Point>::operator+=(std::ptrdiff_t d) -> point_transform_iterator& {
	point_ += d;
	should_load_transformed_point_ = true;
	return *this;
}


template<typename Point>
auto point_transform_iterator<Point>::operator-=(std::ptrdiff_t d) -> point_transform_iterator& {
	point_ -= d;
	should_load_transformed_point_ = true;
	return *this;
}


template<typename Point>
auto point_transform_iterator<Point>::operator+(std::ptrdiff_t d) const -> point_transform_iterator {
	point_transform_iterator it = *this;
	return (it += d);
}

template<typename Point>
auto point_transform_iterator<Point>::operator-(std::ptrdiff_t d) const -> point_transform_iterator {
	point_transform_iterator it = *this;
	return (it -= d);
}

template<typename Point>
std::ptrdiff_t point_transform_iterator<Point>::operator-(const point_transform_iterator& it) const {
	return (point_ - it.point_);
}


template<typename Point>
Point& point_transform_iterator<Point>::operator[](std::ptrdiff_t d) const {
	return point_[d];
}


}