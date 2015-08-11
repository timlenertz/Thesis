#ifndef PCF_CROSS_HISTOGRAM_ERROR_H_
#define PCF_CROSS_HISTOGRAM_ERROR_H_

#include "../correspondences/registration_correspondence.h"
#include "../../geometry/angle.h"
#include "../../geometry/pose.h"
#include <vector>

namespace pcf {

class cross_histogram_error {
public:
	enum mode_t {
		chi_squared,
		correlation,
		bhattacharyya,
		intersection
	};

private:
	float square_side_length_;
	pose fixed_camera_pose_;
	std::size_t num_bins_;
	angle alpha_;
	angle beta_;
	mode_t mode_ = chi_squared;
	bool rejection_;
	float max_weight_;

	std::vector<std::size_t> bin_frequencies_;
	std::size_t total_frequency_;

	float cosa_;
	float tanb_;
	float tan1_;
	float tan2_;


public:
	cross_histogram_error(float p_l, const pose& ps, int num_bins, mode_t mod, bool rej, angle a = angle::degrees(50), angle b = angle::degrees(10), float max_weight = INFINITY);

	cross_histogram_error& operator<<(const point_correspondence_full&);
	cross_histogram_error& operator<<(const cross_histogram_error&);
	
	float operator() () const;
};

}

#endif
