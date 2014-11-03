#include "random.h"
#include <chrono>

namespace pcf {

random_generator& get_random_generator() {
	static random_generator* gen = nullptr;
	if(! gen) {
		auto seed = std::chrono::system_clock::now().time_since_epoch().count();
		gen = new random_generator(seed);
	}
	return *gen;
} 



}
