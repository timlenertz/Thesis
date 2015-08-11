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

private:
	Correspondences correspondences_;
	Error_metric error_metric_;
	const fixed_point_cloud_type& fixed_;
	const loose_point_cloud_type& loose_;

	class receiver {
	public:
		Error_metric error_metric;
		
		receiver(const Error_metric& err) : error_metric(err) { }
		
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
	error_metric_tester(const Correspondences& cor, const Error_metric& err) :
		correspondences_(cor),
		error_metric_(err),
		fixed_(correspondences_.fixed_point_cloud()),
		loose_(correspondences_.loose_point_cloud()) { }

	float operator()(const Eigen::Affine3f& t) const {
		receiver rec(error_metric_);
		correspondences_(rec, t);
		return rec.error_metric();
	}
};


template<typename Correspondences, typename Error_metric>
error_metric_tester<Error_metric, Correspondences> make_error_metric_tester(const Correspondences& cor, const Error_metric& err) {
	error_metric_tester<Error_metric, Correspondences> tester(cor, err);
	return tester;
}

}
}

#endif