#ifndef PCF_FIELD_H_
#define PCF_FIELD_H_

#include <Eigen/Eigen>
#include <memory>
#include <cmath>
#include <set>
#include "gaussian_weight.h"
#include "../geometry/bounding_box.h"
#include "../rgb_color.h"

namespace pcf {

/**
Three-dimensional field where a variable continuously changes value.
Samples can be added to field, and then estimated value for any position in the area can be computed by interpolation. Internally creates Octree of samples.
*/
template<typename Value, typename Weight_func = field_gaussian_weight>
class field {
public:
	using position_type = Eigen::Vector3f;
	
private:
	class node;
	
	std::unique_ptr<node> root_node_;
	Weight_func weight_func_;
	
	Value get_(const position_type&, float& confidence) const;
	void set_(const Value&, const position_type&);

	struct weighted_sample {
		Value value;
		float weight;

		weighted_sample(const Value& val, float w) :
			value(val), weight(w) { }
		bool operator<(const weighted_sample& ws) const {
			return (weight < ws.weight);
		}
	};
	
	using ordered_weighted_samples = std::set<weighted_sample>;

public:
	class handle {
	private:
		field& field_;
		const position_type position_;
		float confidence_;
		Value value_;
		bool loaded_ = false;
	
		void load_();
	
	public:
		explicit handle(field& fld, const position_type& pos);
				
		void operator=(const Value&);
		operator Value ();
		float confidence();
	};
	
	class const_handle {
		float confidence_;
		Value value_;

	public:
		explicit const_handle(const field& fld, const position_type& pos);

		operator Value () const { return value_; }
		float confidence() const { return confidence_; }
	};
	
	std::size_t maximal_interpolation_samples = 1000;

	explicit field(const bounding_box&, const Weight_func& = Weight_func());
	~field();
	
	void clear();
	
	const_handle operator()(const position_type& pos) const { return const_handle(*this, pos); }
	handle operator()(const position_type& pos) { return handle(*this, pos); }
};

extern template class field<unsigned int>;
extern template class field<float>;

using unsigned_int_field = field<unsigned int>;
using float_field = field<float>;

}

#include "field.tcc"

#endif