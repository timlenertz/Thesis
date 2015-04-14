#ifndef PCF_RANDOM_H_
#define PCF_RANDOM_H_

#include <random>

namespace pcf {

/**
The random number generator used by the library.
*/
using random_generator = std::mt19937;

/**
Get global random number generator.
Lazily initialized upon first request, using current system time as seed.
*/
random_generator& get_random_generator();


/**
Generate random integer in [mn, mx].
*/
template<typename T, typename Gen = random_generator>
T random_integer(T mn, T mx, Gen& gen = get_random_generator()) {
	std::uniform_int_distribution<T> dist(mn, mx - 1);
	return dist(gen);
}


/**
Generate random integer in [0, mx].
*/
template<typename T, typename Gen = random_generator>
T random_integer(T mx, Gen& gen = get_random_generator()) {
	return random_integer<T>(0, mx, gen);
}


/**
Generate random real in [mn, mx[.
*/
template<typename T, typename Gen = random_generator>
T random_real(T mn, T mx, Gen& gen = get_random_generator()) {
	std::uniform_real_distribution<T> dist(mn, mx);
	return dist(gen);
}


/**
Iterate from begin to end iterator, picking given number of items at random.
Iterates through the list only once, and Iterator only needs to be an input iterator. Guarantees that expected number of items will be picked, uniformly distributed. Calls callback function for each picked item. List must contain at least that number of items.
*/
template<typename Callback_func, typename Iterator>
void iterate_and_pick_random(Iterator begin, Iterator end, std::size_t total, std::size_t expected, const Callback_func&);


/**
Iterate from begin to end iterator, picking given number of items at random.
*/
template<typename Callback_func, typename Iterator>
void iterate_and_pick_random(Iterator begin, Iterator end, std::size_t expected, const Callback_func&);


}

#include "random.tcc"

#endif