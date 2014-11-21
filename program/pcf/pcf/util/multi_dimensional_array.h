#ifndef PCF_MULTI_DIMENSIONAL_ARRAY_H_
#define PCF_MULTI_DIMENSIONAL_ARRAY_H_

#include "multi_dimensional_buffer.h"
#include "aligned_allocator.h"

namespace pcf {

template<typename T, std::size_t Dim, typename Allocator = aligned_allocator<T>>
class multi_dimensional_array : public multi_dimensional_buffer<T, Dim> {
	using super = multi_dimensional_buffer<T, Dim>;
	
private:
	Allocator allocator_;

public:
	using typename super::multi_size;

	explicit multi_dimensional_array(const multi_size&, const Allocator& alloc = Allocator());
	explicit multi_dimensional_array(const multi_size&, const T& init = T(), const Allocator& alloc = Allocator());
	
	multi_dimensional_array(const multi_dimensional_array&);
	multi_dimensional_array(const multi_dimensional_array&, const Allocator& alloc);
	multi_dimensional_array(multi_dimensional_array&&);
	
	~multi_dimensional_array();
	
	multi_dimensional_array& operator=(const multi_dimensional_array&);
	multi_dimensional_array& operator=(multi_dimensional_array&&);
};


template<typename T> using array_2dim = multi_dimensional_array<T, 2>;
template<typename T> using array_3dim = multi_dimensional_array<T, 3>;

}

#include "multi_dimensional_array.tcc"

#endif