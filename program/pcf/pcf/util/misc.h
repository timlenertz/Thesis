#ifndef PCF_UTIL_MISC_H_
#define PCF_UTIL_MISC_H_

#include <functional>

namespace pcf {


template<typename T>
struct ref_remove_reference_wrapper_ {
    using type = T&;    
};


template<typename T>
struct ref_remove_reference_wrapper_<std::reference_wrapper<T>> {
    using type = T&;    
};


template<typename T> using ref_remove_reference_wrapper
	= typename ref_remove_reference_wrapper_<T>::type;

}

#endif