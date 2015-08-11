#include "mean_absolute_error.h"

namespace pcf {

mean_absolute_error& mean_absolute_error::operator<<(const registration_correspondence& cor) {
	error_sum_ += distance(cor.loose, cor.fixed);
	weight_sum_ += cor.weight;
	return *this;
}


mean_absolute_error& mean_absolute_error::operator<<(const mean_absolute_error& err) {
	error_sum_ += err.error_sum_;
	weight_sum_ += err.weight_sum_;
	return *this;
}


float mean_absolute_error::operator() () const {
	return error_sum_ / weight_sum_;
}

}