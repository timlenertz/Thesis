#include "mean_square_error.h"

namespace pcf {

mean_square_error& mean_square_error::operator<<(const registration_correspondence& cor) {
	error_sum_ += (cor.loose - cor.fixed).squaredNorm();
	weight_sum_ += cor.weight;
	return *this;
}

float mean_square_error::operator() () const {
	return error_sum_ / weight_sum_;
}

}