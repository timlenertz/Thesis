namespace pcf {

template<typename T, std::size_t Dim>
auto multi_dimensional_buffer<T, Dim>::to_size_(const sizes_type& msz) -> std::size_t {
	std::size_t sz = 1;
	for(std::size_t s : msz) sz *= s;
	return sz;
}


template<typename T, std::size_t Dim>
auto multi_dimensional_buffer<T, Dim>::zero_index_() -> indices_type {
	indices_type mi;
	for(std::ptrdiff_t& i : mi) i = 0;
	return mi;
}


template<typename T, std::size_t Dim>
auto multi_dimensional_buffer<T, Dim>::to_offset_(const indices_type& mi) const -> std::ptrdiff_t {
	std::ptrdiff_t idx = 0;
	std::size_t coeff = 1;
	for(std::ptrdiff_t i = Dim - 1; i >= 0; --i) {
		idx += mi[i] * coeff;
		coeff *= sizes_[i];
	}
	return idx;
}

template<typename T, std::size_t Dim>
std::ptrdiff_t multi_dimensional_buffer<T, Dim>::index_to_address(const indices_type& mi) const {
	return to_offset_(mi);
}


template<typename T, std::size_t Dim>
auto multi_dimensional_buffer<T, Dim>::address_to_index(const std::ptrdiff_t& addr) const -> indices_type {
	iterator it = begin();
	it += addr;
	return it.index(); // TODO make better
}



template<typename T, std::size_t Dim>
multi_dimensional_buffer<T, Dim>::multi_dimensional_buffer(const sizes_type& sz, T* data, T* data_end) :
	sizes_(sz), begin_(data), end_(data_end) { }


template<typename T, std::size_t Dim>
bool multi_dimensional_buffer<T, Dim>::in_bounds(const indices_type& mi) const {
	for(std::ptrdiff_t i = 0; i < Dim; ++i)
		if(mi[i] < 0 || mi[i] >= sizes_[i]) return false;
	return true;
}


template<typename T, std::size_t Dim>
auto multi_dimensional_buffer<T, Dim>::move_into_bounds(const indices_type& mi) const -> indices_type {
	indices_type mi2 = mi;
	for(std::ptrdiff_t i = 0; i < Dim; ++i) {
		if(mi[i] < 0) mi2[i] = 0;
		else if(mi[i] >= sizes_[i]) mi2[i] = sizes_[i] - 1;
	}
	return mi2;
}


template<typename T, std::size_t Dim>
T& multi_dimensional_buffer<T, Dim>::operator[](const indices_type& mi) {
	return begin_[ to_offset_(mi) ];
}


template<typename T, std::size_t Dim>
const T& multi_dimensional_buffer<T, Dim>::operator[](const indices_type& mi) const {
	return begin_[ to_offset_(mi) ];
}



template<typename T, std::size_t Dim> template<typename Iterator>
auto multi_dimensional_buffer<T, Dim>::iterator_<Iterator>::operator++() -> iterator_& {
	++ptr_;
	for(std::ptrdiff_t i = Dim - 1; i >= 0; --i) {
		++index_[i];
		if(index_[i] == sizes_[i]) index_[i] = 0;
		else break;
	}
	return *this;
}


template<typename T, std::size_t Dim> template<typename Iterator>
auto multi_dimensional_buffer<T, Dim>::iterator_<Iterator>::operator++(int) -> iterator_ {
	iterator_ old = *this;
	operator++();
	return old;
}


}
