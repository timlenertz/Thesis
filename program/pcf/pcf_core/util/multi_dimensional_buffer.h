#ifndef PCF_MULTI_DIMENSIONAL_BUFFER_H_
#define PCF_MULTI_DIMENSIONAL_BUFFER_H_

#include <array>
#include <type_traits>
#include <iterator>
#include "coordinates.h"

namespace pcf {

/**
Multi-dimensional data buffer.
Uses external buffer, does not allocate memory. Size is specified per dimension, and remains
fixed during object lifetime. Number of dimensions given by Dim template parameter.
Internally serializes elements. Elements where only last index component varies are contiguous.
Access to items using multi-index (one component per dimension, zero-based).
Provides forward iterator that also maintains multi-index, and random access iterator that only
provides access to elements. Both iterate over the elements in the same (internal) order.
*/
template<typename T, std::size_t Dim>
class multi_dimensional_buffer {
public:
	using sizes_type = multi_size<Dim>;
	using indices_type = multi_index<Dim>;
	
protected:
	sizes_type sizes_;
	T* begin_;
	T* end_;
	
	static std::size_t to_size_(const sizes_type&);
	static indices_type zero_index_();
	std::ptrdiff_t to_offset_(const indices_type&) const;

public:
	template<typename It> class iterator_;
	using iterator = iterator_<T*>;
	using const_iterator = iterator_<const T*>;
	
	using raw_iterator = T*;
	using raw_const_iterator = const T*;

	multi_dimensional_buffer(const sizes_type&, T* data, T* data_end);
	multi_dimensional_buffer(const multi_dimensional_buffer&) = default;
		
	T* data() { return begin_; }
	const T* data() const { return begin_; }
	
	T& operator[](const indices_type&);
	const T& operator[](const indices_type&) const;
	
	bool in_bounds(const indices_type&) const;
	indices_type move_into_bounds(const indices_type&) const;
	
	sizes_type size() const { return sizes_; }
	std::size_t size(std::ptrdiff_t i) const { return sizes_[i]; }
	std::size_t total_size() const { return (end_ - begin_); }
	
	std::ptrdiff_t index_to_address(const indices_type&) const;
	indices_type address_to_index(const std::ptrdiff_t&) const; 
	
	iterator begin() { return iterator(sizes_, begin_, zero_index_()); }
	const_iterator begin() const { return const_iterator(sizes_, begin_, zero_index_()); }
	const_iterator cbegin() const { return begin(); }
	
	iterator end() { return iterator(sizes_, end_); }
	const_iterator end() const { return const_iterator(sizes_, end_); }
	const_iterator cend() const { return end(); }
	
	raw_iterator begin_raw() { return begin_; }
	raw_const_iterator begin_raw() const { return begin_; }
	raw_const_iterator cbegin_raw() const { return begin_; }

	raw_iterator end_raw() { return end_; }
	raw_const_iterator end_raw() const { return end_; }
	raw_const_iterator cend_raw() const { return end_; }
};




template<typename T, std::size_t Dim> template<typename Ptr>
class multi_dimensional_buffer<T, Dim>::iterator_ :
public std::iterator<std::forward_iterator_tag, typename std::remove_pointer<Ptr>::type> {
	using super = std::iterator<std::forward_iterator_tag, typename std::remove_pointer<Ptr>::type>;
	using typename super::pointer;
	using typename super::reference;
	
private:
	const sizes_type sizes_;
	Ptr ptr_;
	indices_type index_;

public:
	iterator_(const sizes_type& sz, Ptr ptr, const indices_type& idx = indices_type()) :
		sizes_(sz), ptr_(ptr), index_(idx) { }
	
	const sizes_type& size() const { return sizes_; }
	const indices_type& index() const { return index_; }
	
	reference operator*() const { return *ptr_; }
	pointer operator->() const { return ptr_; }
	
	bool operator==(const iterator_& it) const { return ptr_ == it.ptr_; }
	bool operator!=(const iterator_& it) const { return ptr_ != it.ptr_; }
	
	iterator_& operator++();
	iterator_ operator++(int);
};


}

#include "multi_dimensional_buffer.tcc"

#endif