#include <algorithm>

namespace pcf {

template<typename T, std::size_t Dim, typename Allocator>
multi_dimensional_array<T, Dim, Allocator>::
multi_dimensional_array(const sizes_type& sz, const Allocator& alloc) :
	super(sz, nullptr, nullptr),
	allocator_(alloc)
{
	std::size_t n = super::to_size_(sz);
	super::begin_ = allocator_.allocate(n);
	super::end_ = super::begin_ + n;
}


template<typename T, std::size_t Dim, typename Allocator>
multi_dimensional_array<T, Dim, Allocator>::
multi_dimensional_array(const sizes_type& sz, const T& init, const Allocator& alloc) :
	multi_dimensional_array(sz, alloc)
{
	std::fill(super::begin_, super::end_, init);
}

	
template<typename T, std::size_t Dim, typename Allocator>
multi_dimensional_array<T, Dim, Allocator>::
multi_dimensional_array(const multi_dimensional_array& arr) :
	multi_dimensional_array(arr.sizes_, arr.allocator_)
{
	std::copy(arr.begin_, arr.end_, super::begin_);
}
	


template<typename T, std::size_t Dim, typename Allocator>
multi_dimensional_array<T, Dim, Allocator>::
multi_dimensional_array(const multi_dimensional_array& arr, const Allocator& alloc) :
	multi_dimensional_array(arr.sizes_, alloc)
{
	std::copy(arr.begin_, arr.end_, super::begin_);
}
	


template<typename T, std::size_t Dim, typename Allocator>
multi_dimensional_array<T, Dim, Allocator>::
multi_dimensional_array(multi_dimensional_array&& arr) :
	super(arr.sizes_, arr.begin_),
	allocator_(arr.allocator_)
{
	arr.begin_ = nullptr;
}


template<typename T, std::size_t Dim, typename Allocator>
multi_dimensional_array<T, Dim, Allocator>::~multi_dimensional_array() {
	if(super::begin_) allocator_.deallocate( super::begin_, super::total_size() );
}


template<typename T, std::size_t Dim, typename Allocator>
auto multi_dimensional_array<T, Dim, Allocator>::
operator=(const multi_dimensional_array& arr) -> multi_dimensional_array& {
	if(this == &arr) return *this;
	
	if(arr.total_size() != super::total_size()) {
		// Total sizes differ, need to reallocate buffer
		if(super::begin_) allocator_.deallocate( super::begin_, super::total_size() );
		std::size_t n = arr.total_size();
		super::begin_ = allocator_.allocate(n);
		super::end_ = super::begin_ + n;
	}
	
	super::sizes_ = arr.sizes_;
	std::copy(arr.begin_, arr.end_, super::begin_);

	return *this;
}


template<typename T, std::size_t Dim, typename Allocator>
auto multi_dimensional_array<T, Dim, Allocator>::
operator=(multi_dimensional_array&& arr) -> multi_dimensional_array& {
	if(this == &arr) return *this;
	super::operator=(arr);
	arr.begin_ = nullptr;
	return *this;
}


}