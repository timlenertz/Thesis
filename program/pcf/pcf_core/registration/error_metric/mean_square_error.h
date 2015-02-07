#ifndef PCF_REGISTRATION_MEAN_SQUARE_ERROR_H_
#define PCF_REGISTRATION_MEAN_SQUARE_ERROR_H_

#include "../correspondences/registration_correspondence.h"

namespace pcf {

class mean_square_error {
private:
	float error_sum_ = 0;
	float weight_sum_ = 0;

public:
	mean_square_error& operator<<(const registration_correspondence&);
	mean_square_error& operator<<(const mean_square_error&);
	
	float operator() () const;
};

}

#endif