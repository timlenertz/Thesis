namespace pcf {

template<typename T, std::size_t Dim>
auto multi_dimensional_array<T, Dim>::to_size_(const multi_size& msz) -> std::size_t {
	std::size_t sz = 1;
	for(std::size_t s : msz) sz *= s;
	return sz;
}


template<typename T, std::size_t Dim>
auto multi_dimensional_array<T, Dim>::zero_index_() -> multi_index {
	multi_index mi;
	for(std::ptrdiff_t& i : mi) i = 0;
	return mi;
}


template<typename T, std::size_t Dim>
auto multi_dimensional_array<T, Dim>::to_offset_(const multi_index& mi) const -> std::ptrdiff_t {
	std::ptrdiff_t idx = 0;
	std::size_t coeff = 1;
	for(std::ptrdiff_t i = Dim - 1; i >= 0; --i) {
		idx += mi[i] * coeff;
		coeff *= sizes_[i];
	}
	return idx;
}


template<typename T, std::size_t Dim>
multi_dimensional_array<T, Dim>::multi_dimensional_array(const multi_size& sz) :
	data_( to_size_(sz) ), sizes_(sz) { }

template<typename T, std::size_t Dim>
multi_dimensional_array<T, Dim>::multi_dimensional_array(const multi_size&, T def) :
	data_( to_size_(sz), def ), sizes_(sz) { }



template<typename T, std::size_t Dim>
T& multi_dimensional_array<T, Dim>::operator[](const multi_index& mi) {
	return data_[ to_offset_(mi) ];
}


template<typename T, std::size_t Dim>
const T& multi_dimensional_array<T, Dim>::operator[](const multi_index& mi) const {
	return data_[ to_offset_(mi) ];
}



template<typename T, std::size_t Dim> template<bool Const>
auto multi_dimensional_array<T, Dim>::iterator_<Const>::operator++() -> iterator_& {
	++ptr_;
	for(std::ptrdiff_t i = Dim - 1; i >= 0; --i) {
		++index_[i];
		if(index_[i] == sizes_[i]) index_[i] = 0;
		else break;
	}
	return *this;
}


template<typename T, std::size_t Dim> template<bool Const>
auto multi_dimensional_array<T, Dim>::iterator_<Const>::operator++(int) -> iterator_ {
	iterator_ old = *this;
	operator++();
	return old;
}


}
