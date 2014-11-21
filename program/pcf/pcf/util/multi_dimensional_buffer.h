#ifndef PCF_MULTI_DIMENSIONAL_BUFFER_H_
#define PCF_MULTI_DIMENSIONAL_BUFFER_H_

#include <array>
#include <type_traits>
#include <iterator>

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
	using multi_index = std::array<std::ptrdiff_t, Dim>;
	using multi_size = std::array<std::size_t, Dim>;
	
protected:
	multi_size sizes_;
	T* begin_;
	T* end_;
	
	static std::size_t to_size_(const multi_size&);
	static multi_index zero_index_();
	std::ptrdiff_t to_offset_(const multi_index&) const;

public:
	template<typename It> class iterator_;
	using iterator = iterator_<T*>;
	using const_iterator = iterator_<const T*>;
	
	using raw_iterator = T*;
	using raw_const_iterator = const T*;

	multi_dimensional_buffer(const multi_size&, T* data);
	multi_dimensional_buffer(const multi_dimensional_buffer&) = default;
		
	T* data() { return begin_; }
	const T* data() const { return begin_; }
	
	T& operator[](const multi_index&);
	const T& operator[](const multi_index&) const;
	
	multi_size size() const { return sizes_; }
	std::size_t total_size() const { return (end_ - begin_); }
	
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
	const multi_size sizes_;
	Ptr ptr_;
	multi_index index_;

public:
	iterator_(const multi_size& sz, Ptr ptr, const multi_index& idx = multi_index()) :
		sizes_(sz), ptr_(ptr), index_(idx) { }
	
	const multi_size& size() const { return sizes_; }
	const multi_index& index() const { return index_; }
	
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