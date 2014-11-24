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
void iterative_correspondences_registration<Correspondences, Transformation_estimation, Error_metric>::iteration() {
	receiver rec;	
	correspondences_(rec);
	
	error_ = rec.error_metric();
	estimated_transformation_ = rec.transformation_estimation().inverse();
	
	std::cout << error_ << " -- " << rec.count << std::endl;
}


template<typename Correspondences, typename Transformation_estimation, typename Error_metric>
void iterative_correspondences_registration<Correspondences, Transformation_estimation, Error_metric>::run() {
	iteration();
	
	std::size_t iteration_count = 1;	
	while(iteration_count++ < maximal_iterations && error_ > minimal_error) {
		float previous_error = error_;
		Eigen::Affine3f previous_estimated_transformation = estimated_transformation_;
	
		loose_.apply_transformation(estimated_transformation_);
		iteration();
		
		if(error_ > previous_error) {
			loose_.apply_transformation(previous_estimated_transformation.inverse());
			break;
		}
		
		previous_estimated_transformation = estimated_transformation_;
		previous_error = error_;
	}
}


}
