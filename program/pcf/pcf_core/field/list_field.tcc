#include <iostream>

namespace pcf {

template<typename Value, typename Weight_func>
Value list_field<Value, Weight_func>::get_(const Eigen::Vector3f& pos, float& confidence) const {
	float total_weight = 0.0;
	Value weighted_sum = Value();
	
	#pragma omp parallel
	{
		float total_weight_part = 0.0;
		Value weighted_sum_part = Value();

		#pragma omp for
		for(auto it = samples_.begin(); it < samples_.end(); ++it) {
			const sample& samp = *it;
			float weight = super::weight_func_(pos, samp.position);
			total_weight_part += weight;
			weighted_sum_part += weight * samp.value;
		}
		
		#pragma omp critical
		{
			total_weight += total_weight_part;
			weighted_sum += weighted_sum_part;
		}
	}
	
	return weighted_sum / total_weight;
}


template<typename Value, typename Weight_func>
void list_field<Value, Weight_func>::set_(const Value& sample_value, const Eigen::Vector3f& sample_position) {
	samples_.emplace_back(sample_value, sample_position);
}


template<typename Value, typename Weight_func>
void list_field<Value, Weight_func>::clear() {
	samples_.clear();
}


}