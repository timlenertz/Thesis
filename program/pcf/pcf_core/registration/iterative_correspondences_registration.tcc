#include <iostream>
#include <cmath>

namespace pcf {

template<typename Correspondences, typename Transformation_estimation, typename Error_metric>
class iterative_correspondences_registration<Correspondences, Transformation_estimation, Error_metric>::receiver {
public:
	Transformation_estimation transformation_estimation;
	Error_metric error_metric;
	std::size_t count = 0;
	
	receiver& operator<<(const receiver& rec) {
		transformation_estimation << rec.transformation_estimation;
		error_metric << rec.error_metric;
		count += rec.count;
		return *this;			
	}
	
	template<typename Obj>
	receiver& operator<<(const Obj& obj) {
		transformation_estimation << obj;
		error_metric << obj;
		++count;
		return *this;
	}
};



template<typename Correspondences, typename Transformation_estimation, typename Error_metric>
void iterative_correspondences_registration<Correspondences, Transformation_estimation, Error_metric>::compute_estimated_transformation_and_error() {
	receiver rec;
	correspondences(rec, accumulated_transformation_);
	
	current_error_ = rec.error_metric();
	estimated_transformation_ = rec.transformation_estimation();
}


}
