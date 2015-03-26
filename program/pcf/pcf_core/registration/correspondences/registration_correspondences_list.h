#ifndef PCF_REGISTRATION_CORRESPONDENCES_LIST_H_
#define PCF_REGISTRATION_CORRESPONDENCES_LIST_H_

#include <Eigen/Eigen>
#include <deque>
#include "registration_correspondence.h"
#include "../../space_object_wrapper.h"

namespace pcf {

class registration_correspondences_list {
private:
	std::deque<registration_correspondence> cors_;

public:
	using iterator = std::deque<registration_correspondence>::const_iterator;
	using const_iterator = iterator;

	void clear() { cors_.clear(); }
	
	const_iterator begin() const { return cors_.begin(); }
	const_iterator end() const { return cors_.end(); }
	std::size_t size() const { return cors_.size(); }

	registration_correspondences_list& operator<<(const registration_correspondence& c) {
		cors_.push_back(c);
		return *this;
	}
	
	registration_correspondences_list& operator<<(const registration_correspondences_list& lst) {
		cors_.insert(cors_.end(), lst.cors_.begin(), lst.cors_.end());
		return *this;
	}
};


using space_registration_correspondences_list = space_object_wrapper<registration_correspondences_list>;

}

#endif
