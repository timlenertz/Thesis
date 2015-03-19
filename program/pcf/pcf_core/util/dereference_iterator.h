#ifndef PCF_DEREFERENCE_ITERATOR_H_
#define PCF_DEREFERENCE_ITERATOR_H_

#include <type_traits>
#include <utility>

namespace pcf {

template<typename It>
class dereference_iterator {
public:
	using value_type = typename std::remove_reference<decltype(**std::declval<It>())>::type;
	using difference_type = std::ptrdiff_t;
	using pointer = value_type*;
	using reference = value_type&;
	using iterator_category = std::random_access_iterator_tag;
	
private:
	It it_;
	
public:
	explicit dereference_iterator(const It& it) : it_(it) { }
	dereference_iterator(const dereference_iterator&) = default;
	
	dereference_iterator& operator=(const dereference_iterator&) = default;
	
	const It& underlying_iterator() const { return it_; }
	It& underlying_iterator() { return it_; }
	
	bool operator==(const dereference_iterator& dt) const { return it_ == dt.it_; }
	bool operator!=(const dereference_iterator& dt) const { return it_ != dt.it_; }
	bool operator<(const dereference_iterator& dt) const { return it_ < dt.it_; }
	bool operator<=(const dereference_iterator& dt) const { return it_ <= dt.it_; }
	bool operator>(const dereference_iterator& dt) const { return it_ > dt.it_; }
	bool operator>=(const dereference_iterator& dt) const { return it_ >= dt.it_; }
	
	reference operator*() const { return **it_; }
	pointer operator->() const { return &(**it_); }
	
	dereference_iterator& operator++() { ++it_; return *this; }
	dereference_iterator& operator--() { --it_; return *this; }
	dereference_iterator operator++(int) { return dereference_iterator(it_++); }
	dereference_iterator operator--(int) { return dereference_iterator(it_--); }
	
	dereference_iterator& operator+=(std::ptrdiff_t i) { it_ += i; }
	dereference_iterator& operator-=(std::ptrdiff_t i) { it_ += i; }
	
	dereference_iterator operator+(std::ptrdiff_t i) const { return dereference_iterator(it_ + i); }
	dereference_iterator operator-(std::ptrdiff_t i) const { return dereference_iterator(it_ - i); }
	
	std::ptrdiff_t operator-(const dereference_iterator& dt) const { return (it_ - dt.it_); }
	
	reference operator[](std::ptrdiff_t i) const { return *(it_[i]); }
};

}

#endif
