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
void iterative_correspondences_registration<Correspondences, Transformation_estimation, Error_metric>::estimate_transformation() {
	receiver rec;
	correspondences(rec);
	
	error_ = rec.error_metric();
	estimated_transformation_ = rec.transformation_estimation();
}


template<typename Correspondences, typename Transformation_estimation, typename Error_metric>
void iterative_correspondences_registration<Correspondences, Transformation_estimation, Error_metric>::apply_estimated_transformation() {
	loose.transform( estimated_transformation_.inverse() );
}



template<typename Correspondences, typename Transformation_estimation, typename Error_metric>
void iterative_correspondences_registration<Correspondences, Transformation_estimation, Error_metric>::
run(const iteration_callback& cb) {
	estimate_transformation();
	
	std::size_t iteration_count = 1;	
	while(iteration_count++ < maximal_iterations && error_ > minimal_error) {
		pose previous_pose = loose.relative_pose();
		float previous_error = error_;

		apply_estimated_transformation();
		
		if(cb) cb(iteration_state(estimated_transformation_, error_), false);
		
		estimate_transformation();
		
		if(error_ > previous_error && stop_on_divergence) {
			loose.set_relative_pose(previous_pose);
			break;
		}
	}
	if(cb) cb(iteration_state(estimated_transformation_, error_), true);
}


template<typename Correspondences, typename Transformation_estimation, typename Error_metric>
auto iterative_correspondences_registration<Correspondences, Transformation_estimation, Error_metric>::
run_and_get_statistics(const iteration_callback& cb) -> std::vector<iteration_state> {
	std::vector<iteration_state> stat;
	run([&stat](const iteration_state& st, bool done) {
		stat.push_back(st);
	});
	return stat;
}

}
