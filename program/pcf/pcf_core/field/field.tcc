#include <iostream>

namespace pcf {


template<typename Value, typename Weight_func>
field<Value, Weight_func>::field(const Weight_func& func) :
	weight_func_(func) { }


template<typename Value, typename Weight_func>
field<Value, Weight_func>::~field() { }


template<typename Value, typename Weight_func>
void field<Value, Weight_func>::clear() { }


template<typename Value, typename Weight_func>
inline field<Value, Weight_func>::handle::handle(field& fld, const Eigen::Vector3f& pos) :
	field_(fld), position_(pos) { }


template<typename Value, typename Weight_func>
inline void field<Value, Weight_func>::handle::load_() {
	value_ = field_.get_(position_, confidence_);
	loaded_ = true;
}


template<typename Value, typename Weight_func>
inline void field<Value, Weight_func>::handle::operator=(const Value& val) {
	field_.set_(val, position_);
}


template<typename Value, typename Weight_func>
inline field<Value, Weight_func>::handle::operator Value() {
	if(! loaded_) load_();
	return value_;
}


template<typename Value, typename Weight_func>
inline float field<Value, Weight_func>::handle::confidence() {
	if(! loaded_) load_();
	return confidence_;
}


template<typename Value, typename Weight_func>
inline field<Value, Weight_func>::const_handle::const_handle(const field& fld, const Eigen::Vector3f& pos) :
	value_(fld.get_(pos, confidence_)) { }


}