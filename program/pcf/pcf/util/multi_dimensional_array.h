#ifndef PCF_MULTI_DIMENSIONAL_ARRAY_H_
#define PCF_MULTI_DIMENSIONAL_ARRAY_H_

#include <array>
#include <vector>
#include <type_traits>
#include <iterator>
#include <pair>

namespace pcf {

template<typename T, std::size_t Dim>
class multi_dimensional_array {
public:
	using multi_index = std::array<std::ptrdiff_t, Dim>;
	using multi_size = std::array<std::size_t, Dim>;
	
private:
	std::vector<T> data_;
	multi_size sizes_;
	
	static std::size_t to_size_(const multi_size&);
	static multi_index zero_index_();
	std::ptrdiff_t to_offset_(const multi_index&) const;

public:
	template<bool Const> class iterator_;
	using iterator = iterator_<false>;
	using const_iterator = iterator_<true>;

	explicit multi_dimensional_array(const multi_size&);
	multi_dimensional_array(const multi_size&, T def);
	multi_dimensional_array(const multi_dimensional_array&) = default;
	multi_dimensional_array(multi_dimensional_array&&) = default;
	
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




template<typename T, std::size_t Dim> template<bool Const>
class multi_dimensional_array<T, Dim>::iterator_ :
public std::iterator<std::forward_iterator_tag, std::conditional<Const, const T, T>::type> {
	using super = std::iterator<std::forward_iterator_tag, std::conditional<Const, const T, T>::type>;
	using typename super::pointer;
	
private:
	const multi_size sizes_;
	pointer ptr_;
	multi_index index_;

public:
	iterator_(const multi_size& sz, pointer p, const multi_index& idx = multi_index()) :
		sizes_(sz), ptr_(p), index_(idx) { }
	
	iterator_(const iterator_<false>& it) :
		sizes_(it.size()), index_(it.index()), ptr_(it.ptr()) { } 
	
	const multi_size& size() const { return sizes_; }
	const multi_index& index() const { return index_; }
	pointer ptr() const { return ptr_; }
	
	reference operator*() const { return *ptr_; }
	pointer operator->() const { return ptr_; }
	
	template<bool Other_const>
	bool operator==(const iterator_<Other_const>& it) const { return ptr_ == it.ptr(); }

	template<bool Other_const>
	bool operator!=(const iterator_<Other_const>& it) const { return ptr_ != it.ptr(); }
	
	iterator_& operator++();
	iterator_ operator++(int);
};



template<typename T> using array_2dim = multi_dimensional_array<T, 2>;
template<typename T> using array_3dim = multi_dimensional_array<T, 3>;


}

#include "multi_dimensional_array.tcc"

#endif