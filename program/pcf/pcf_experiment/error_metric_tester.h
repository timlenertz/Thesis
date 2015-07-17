#ifndef PCFEX_ERROR_METRIC_TESTER_H_
#define PCFEX_ERROR_METRIC_TESTER_H_

#include "../pcf_core/registration/error_metric/mean_absolute_error.h"

namespace pcf {
namespace exper {

template<typename Error_metric, typename Correspondences>
class error_metric_tester {
public:
	using correspondence_type = typename Correspondences::correspondence_type;
	using fixed_point_cloud_type = typename Correspondences::fixed_point_cloud_type;
	using loose_point_cloud_type = typename Correspondences::loose_point_cloud_type;

	Correspondences correspondences;	
	const fixed_point_cloud_type& fixed;
	const loose_point_cloud_type& loose;

	class receiver {
	public:
		Error_metric error_metric;
		
		receiver& operator<<(const receiver& rec) {
			error_metric << rec.error_metric;
			return *this;
		}
		
		template<typename Obj>
		receiver& operator<<(const Obj& obj) {
			error_metric << obj;
			return *this;
		}
	};

public:
	explicit error_metric_tester(const Correspondences& cor) :
		correspondences(cor),
		fixed(correspondences.fixed_point_cloud()),
		loose(correspondences.loose_point_cloud()) { }

	float operator()(const Eigen::Affine3f& t) const {
		receiver rec;
		correspondences(rec, t);
		return rec.error_metric();
	}
};


template<typename Correspondences>
error_metric_tester<mean_absolute_error, Correspondences> make_error_metric_tester(const Correspondences& cor) {
	error_metric_tester<mean_absolute_error, Correspondences> tester(cor);
	return tester;
}

}
}

#endif