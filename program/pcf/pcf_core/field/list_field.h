#ifndef PCF_LIST_FIELD_H_
#define PCF_LIST_FIELD_H_

#include <Eigen/Eigen>
#include <vector>
#include "field.h"

namespace pcf {

/**
Three-dimensional field where a variable continuously changes value.
Samples can be added to field, and then estimated value for any position in the area can be computed by interpolation. 
*/
template<typename Value, typename Weight_func = field_gaussian_weight>
class list_field : public field<Value, Weight_func> {
public:
	using super = field<Value, Weight_func>;
		
private:
	struct sample {
		Value value;
		Eigen::Vector3f position;

		sample(const Value& val, const Eigen::Vector3f& pos) :
			value(val), position(pos) { }
	};

	std::vector<sample> samples_;
	
	Value get_(const position_type&, float& confidence) const override;
	void set_(const Value&, const position_type&) override;

public:
	explicit field(const Weight_func& wf = Weight_func()) : field(wf) { }
	~field() { }
	
	void clear();
};

extern template class list_field<unsigned int>;
extern template class list_field<float>;

}

#include "list_field.tcc"

#endif