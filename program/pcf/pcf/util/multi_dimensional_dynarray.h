#ifndef PCF_MULTI_DIMENSIONAL_DYNARRAY_H_
#define PCF_MULTI_DIMENSIONAL_DYNARRAY_H_

#include <array>
#include <vector>
#include <type_traits>
#include <iterator>

namespace pcf {

template<typename T, std::size_t Dim>
class multi_dimensional_dynarray {
public:
	using multi_index = std::array<std::ptrdiff_t, Dim>;
	using multi_size = std::array<std::size_t, Dim>;
	
private:
	using data_vector = std::vector<T>;
	data_vector data_;
	multi_size sizes_;
	
	static std::size_t to_size_(const multi_size&);
	static multi_index zero_index_();
	std::ptrdiff_t to_offset_(const multi_index&) const;

public:
	template<typename It> class iterator_;
	using iterator = iterator_<typename data_vector::iterator>;
	using const_iterator = iterator_<typename data_vector::const_iterator>;

	explicit multi_dimensional_dynarray(const multi_size&);
	multi_dimensional_dynarray(const multi_size&, T def);
	multi_dimensional_dynarray(const multi_dimensional_dynarray&) = default;
	multi_dimensional_dynarray(multi_dimensional_dynarray&&) = default;
	
	T* data() { return data_.data(); }
	const T* data() const { return data_.data(); }
	
	T& operator[](const multi_index&);
	const T& operator[](const multi_index&) const;
	
	multi_size size() const { return sizes_; }
	std::size_t total_size() const { return data_.size(); }
	
	iterator begin() { return iterator(sizes_, data_.begin(), zero_index_()); }
	const_iterator begin() const { return const_iterator(sizes_, data_.cbegin(), zero_index_()); }
	const_iterator cbegin() const { return begin(); }
	
	iterator end() { return iterator(sizes_, data_.end()); }
	const_iterator end() const { return const_iterator(sizes_, data_.cend()); }
	const_iterator cend() const { return end(); }
};




template<typename T, std::size_t Dim> template<typename Iterator>
class multi_dimensional_dynarray<T, Dim>::iterator_ :
public std::iterator<std::forward_iterator_tag, typename Iterator::value_type> {
	using super = std::iterator<std::forward_iterator_tag, typename Iterator::value_type>;
	using typename super::pointer;
	using typename super::reference;
	
private:
	const multi_size sizes_;
	Iterator data_iterator_;
	multi_index index_;

public:
	iterator_(const multi_size& sz, Iterator dit, const multi_index& idx = multi_index()) :
		sizes_(sz), data_iterator_(dit), index_(idx) { }
	
	const multi_size& size() const { return sizes_; }
	const multi_index& index() const { return index_; }
	
	reference operator*() const { return *data_iterator_; }
	pointer operator->() const { return data_iterator_->operator->(); }
	
	bool operator==(const iterator_& it) const { return data_iterator_ == it.data_iterator_; }
	bool operator!=(const iterator_& it) const { return data_iterator_ != it.data_iterator_; }
	
	iterator_& operator++();
	iterator_ operator++(int);
};



template<typename T> using dynarray2 = multi_dimensional_dynarray<T, 2>;
template<typename T> using dynarray3 = multi_dimensional_dynarray<T, 3>;


}

#include "multi_dimensional_dynarray.tcc"

#endif