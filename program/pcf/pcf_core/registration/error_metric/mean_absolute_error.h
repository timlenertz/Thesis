#ifndef PCF_REGISTRATION_MEAN_ABSOLUTE_ERROR_H_
#define PCF_REGISTRATION_MEAN_ABSOLUTE_ERROR_H_

#include "../correspondences/registration_correspondence.h"

namespace pcf {

/**
Mean absolute error metric.
Computes mean of unsigned euclidian distances of corresponding point pairs.
*/
class mean_absolute_error {
private:
	float error_sum_ = 0;
	float weight_sum_ = 0;

public:
	mean_absolute_error& operator<<(const registration_correspondence&);
	mean_absolute_error& operator<<(const mean_absolute_error&);
	
	float operator() () const;
};

}

#endif