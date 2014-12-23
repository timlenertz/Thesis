#ifndef PCF_SKIP_POINT_FILTER_H_
#define PCF_SKIP_POINT_FILTER_H_

#include <atomic>
#include "point_filter.h"

namespace pcf {

class skip_point_filter : public point_filter {
private:
	const std::size_t k_;
	std::size_t counter_ = 0;
	
public:
	skip_point_filter(std::size_t k) :
	k_(k) {	}
	
	void reset() {
		counter_ = 0;
	}

	bool operator()(const point_xyz&) {
		if(++counter_ == k_) {
			counter_ = 0;
			return false;
		} else {
			return true;
		}
	}
};


}

#endif
