#include "cross_histogram_error.h"
#include "../../geometry/math_constants.h"
#include "../../util/misc.h"
#include <cmath>

namespace pcf {

cross_histogram_error::cross_histogram_error(float p_l, const pose& ps, int num_bins, mode_t mod, bool rej, angle a, angle b, float max_weight) :
	square_side_length_(p_l),
	fixed_camera_pose_(ps),
	num_bins_(num_bins),
	alpha_(a), beta_(b),
	mode_(mod),
	rejection_(rej),
	max_weight_(max_weight),
	bin_frequencies_(num_bins, 0),
	total_frequency_(0),
	cosa_(std::cos(a)),
	tanb_(std::tan(b)),
	tan1_(std::tan(quarter_pi - b)),
	tan2_(std::tan(quarter_pi + b)) { }


cross_histogram_error& cross_histogram_error::operator<<(const point_correspondence_full& cor) {
	const Eigen::Vector3f& p = cor.fixed; // model
	const Eigen::Vector3f& pn = cor.fixed_normal;
	const Eigen::Vector3f& q = cor.loose; // sample
	const Eigen::Vector3f& qn = cor.loose_normal;
	
	float d = distance(p, q);
	
	Eigen::Vector3f n = fixed_camera_pose_.transformation_from_world().linear() * qn;
	float nx = n[0], ny = n[1], nz = n[2];
		
	if(not(std::abs(nz) > cosa_)) {
		float r  = std::abs(nx / ny);
		float ir = std::abs(ny / nx);
		if(not(r < tanb_ && ir < tanb_ && tan1_ < r && r < tan2_)) return *this;
	}
	
	if(rejection_) {
		Eigen::Vector3f diff = q - p;
		Eigen::Vector3f proj_q = q - pn.dot(diff)*pn;
		d = (p - proj_q).norm();
	}

	float lmin = square_side_length_ * std::min({
		std::sqrt(1.0 + sq(nx)/sq(nz)),
		std::sqrt(1.0 + sq(ny)/sq(nz)),
		std::sqrt(2.0 + sq(nx+ny)/sq(nz)),
		std::sqrt(2.0 + sq(nx-ny)/sq(nz))
	});
	
	float adj_d = 2.0 * d / lmin;
	
	std::ptrdiff_t i = num_bins_ * adj_d;
	if(i >= num_bins_) return *this;
		
	bin_frequencies_[i]++;
	total_frequency_++;
	
	return *this;
}


cross_histogram_error& cross_histogram_error::operator<<(const cross_histogram_error& err) {
	for(std::ptrdiff_t i = 0; i < num_bins_; ++i)
		bin_frequencies_.at(i) += err.bin_frequencies_.at(i);
	total_frequency_ += err.total_frequency_;
	return *this;
}


float cross_histogram_error::operator() () const {
	float bin_range = 1.0 / num_bins_;
	float bin_center = bin_range / 2.0;
	
	if(mode_ == chi_squared) {
		float chi_sq = 0;
		for(std::ptrdiff_t i = 0; i < num_bins_; ++i) {
			float o = float(bin_frequencies_[i]) / total_frequency_;
			float e = bin_range * bin_center * 2.0;
			chi_sq += sq(o - e) / e;
			bin_center += bin_range;
		}
		return chi_sq;

	} else if(mode_ == correlation) {
		float num = 0, var1 = 0, var2 = 0;
		float mean = bin_range;
		for(std::ptrdiff_t i = 0; i < num_bins_; ++i) {
			float o = float(bin_frequencies_[i]) / total_frequency_;
			float e = bin_range * bin_center * 2.0;

			num += (o - mean)*(e - mean);
			var1 += sq(o - mean);
			var2 += sq(e - mean);

			bin_center += bin_range;
		}
		return num / std::sqrt(var1 * var2);
	
	} else if(mode_ == bhattacharyya) {
		float sum = 0;
		for(std::ptrdiff_t i = 0; i < num_bins_; ++i) {
			float o = float(bin_frequencies_[i]) / total_frequency_;
			float e = bin_range * bin_center * 2.0;
			sum += std::sqrt(o * e);
			bin_center += bin_range;
		}
		return std::sqrt(1.0 - sum/num_bins_);
		
	} else if(mode_ == intersection) {
		float sum = 0;
		for(std::ptrdiff_t i = 0; i < num_bins_; ++i) {
			float o = float(bin_frequencies_[i]) / total_frequency_;
			float e = bin_range * bin_center * 2.0;
			sum += std::min(o, e);
			bin_center += bin_range;
		}
		return sum;

	} else {
		return NAN;
	}
}

}