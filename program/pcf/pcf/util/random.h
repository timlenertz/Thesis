#ifndef PCF_RANDOM_H_
#define PCF_RANDOM_H_

#include <random>

namespace pcf {

using random_generator = std::mt19937;

random_generator& get_random_generator();


template<typename T> T random_integer(T mn, T mx) {
	std::uniform_int_distribution<T> dist(mn, mx - 1);
	return dist( get_random_generator() );
}


template<typename T> T random_integer(T mx) {
	return random_integer<T>(0, mx);
}


template<typename T> T random_real(T mn, T mx) {
	std::uniform_real_distribution<T> dist(mn, mx);
	return dist( get_random_generator() );
}

}

#endif