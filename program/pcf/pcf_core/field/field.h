#ifndef PCF_FIELD_H_
#define PCF_FIELD_H_

#include <Eigen/Eigen>
#include <memory>
#include <cmath>
#include "gaussian_weight.h"

namespace pcf {

/**
Three-dimensional field where a variable continuously changes value.
Samples can be added to field, and then estimated value for any position in the area can be computed by interpolation. 
*/
template<typename Value, typename Weight_func = field_gaussian_weight>
class field {
public:
	using position_type = Eigen::Vector3f;
	
protected:
	Weight_func weight_func_;
	
	virtual Value get_(const position_type&, float& confidence) const = 0;
	virtual void set_(const Value&, const position_type&) = 0;

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
	
protected:
	explicit field(const Weight_func& = Weight_func());

public:
	virtual ~field();

	virtual void clear();
	
	const_handle operator()(const position_type& pos) const { return const_handle(*this, pos); }
	handle operator()(const position_type& pos) { return handle(*this, pos); }
};

}

#include "field.tcc"

#endif