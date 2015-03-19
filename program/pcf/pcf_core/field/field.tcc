#include <iostream>

namespace pcf {

template<typename Value, typename Weight_func>
class field<Value, Weight_func>::node {
private:
	using children_array = std::array<std::unique_ptr<node>, 8>;

	const bounding_box box_;
	mutable Value sample_value_;
	mutable Eigen::Vector3f sample_position_;
	mutable bool need_to_mix_ = false;
	children_array children_;
	std::size_t number_of_samples_;
	
	std::ptrdiff_t child_index_for_position_(const Eigen::Vector3f&) const;
	bounding_box child_bounding_box_(std::ptrdiff_t i) const;

	void mix_(const Weight_func&) const;

public:
	node(const bounding_box& box);
	node(const node&) = delete;
	node& operator=(const node&) = delete;
	
	const bounding_box& box() const { return box_; }
	
	void add(const Value&, const Eigen::Vector3f&);
	
	ordered_weighted_samples collect_samples_around_position(const Eigen::Vector3f&, std::size_t max_samples, const Weight_func&) const;
};



template<typename Value, typename Weight_func>
field<Value, Weight_func>::node::node(const bounding_box& box) :
	box_(box),
	number_of_samples_(0) { }
	

template<typename Value, typename Weight_func>
void field<Value, Weight_func>::node::add(const Value& new_sample_value, const Eigen::Vector3f& new_sample_position) {
	if(number_of_samples_ == 0) {
		// Node is leaf and still empty.
		//Set to leaf with the sample.
		sample_value_ = new_sample_value;
		sample_position_ = new_sample_position;
		number_of_samples_ = 1;
		
	} else {
		if(number_of_samples_ == 1) {
			// Node is leaf.
			// Put its existing sample in a new child for it.
			std::ptrdiff_t i = child_index_for_position_(sample_position_);
			children_[i].reset(new node(child_bounding_box_(i)));
			children_[i]->add(sample_value_, sample_position_);
		}
		
		// Put the new sample in a child (new, or recursively)
		std::ptrdiff_t i = child_index_for_position_(new_sample_position);
		auto& c = children_[i];
		if(c == nullptr) c.reset(new node(child_bounding_box_(i)));
		c->add(new_sample_value, new_sample_position);
		
		// Lazily mix child samples to create combined sample,
		// when estimated value is requested.
		need_to_mix_ = true;
		
		++number_of_samples_;
	}
}


template<typename Value, typename Weight_func>
void field<Value, Weight_func>::node::mix_(const Weight_func& weight_func) const {
	Eigen::Vector3f position_sum = Eigen::Vector3f::Zero();
	std::size_t number_of_children = 0;
	for(auto& c : children_) if(c) {
		if(c->need_to_mix_) c->mix_(weight_func);
		position_sum += c->sample_position_;
		++number_of_children;
	}
	sample_position_ = position_sum / number_of_children;

	Value weighted_sum = Value();
	float total_weight = 0.0;	
	for(auto& c : children_) if(c) {
		float weight = weight_func(sample_position_, c->sample_position_);
		weighted_sum = weighted_sum + weight*c->sample_value_;
		total_weight += weight;
	}
	sample_value_ = (1.0f/total_weight) * weighted_sum;
	
	need_to_mix_ = false;
}

template<typename Value, typename Weight_func>
auto field<Value, Weight_func>::node::collect_samples_around_position
(const Eigen::Vector3f& pos, std::size_t max_samples, const Weight_func& weight_func) const -> ordered_weighted_samples {	
	auto make_sample = [&](const node& nd) {
		float weight = weight_func(pos, nd.sample_position_);
		return weighted_sample(nd.sample_value_, weight);
	};
		
	// Handle leaf
	if(number_of_samples_ == 0)	{
		// Empty node, return nothing
		return ordered_weighted_samples();
	} else if(number_of_samples_ == 1) {
		// Just one sample in this node, return it.
		return { make_sample(*this) };
	}
	
	ordered_weighted_samples samples;

	std::ptrdiff_t i = child_index_for_position_(pos);
	const auto& c = children_[i];
	if(c) {
		// Recursive descent to deepest child node containing the position
		// Leaf's sample gets always added, plus some surrounding...
		samples = c->collect_samples_around_position(pos, max_samples, weight_func);
	} else {
		// No child at that direction, add mixed sample for this node instead.
		// Not a leaf, so mix when not done yet.
		if(need_to_mix_) mix_(weight_func);
		samples.insert( make_sample(*this) );
	}
	
	// Also add mixed samples from the surrounding children, but without descending
	for(std::ptrdiff_t j = 0; j < children_.size(); ++j) {
		if(i == j) continue;
		const auto& c = children_[j];
		if(! c) continue;
		
		if(c->need_to_mix_) c->mix_(weight_func);
		
		float max_weight = samples.rbegin()->weight;
		
		weighted_sample sample = make_sample(*c);
		if(samples.size() < max_samples) {
			// Not yet filled max_samples, add this one.
			samples.insert(sample);
		} else if(sample.weight > max_weight) {
			// Already have max_samples.
			// If this one's weight is greater than the maximal weight,
			// remove the one with minimal weight and add this one instead.
			samples.erase(samples.begin()); // Minimal weight is at first pos.
			samples.insert(sample);
		}
	}
	
	return samples;
}


template<typename Value, typename Weight_func>
std::ptrdiff_t field<Value, Weight_func>::node::child_index_for_position_(const Eigen::Vector3f& p) const {
	Eigen::Vector3f c = box_.center();
	std::ptrdiff_t i = 0;
	if(p[0] >= c[0]) i += 1;
	if(p[1] >= c[1]) i += 2;
	if(p[2] >= c[2]) i += 4;
	return i;
}



template<typename Value, typename Weight_func>
bounding_box field<Value, Weight_func>::node::child_bounding_box_(std::ptrdiff_t i) const {
	Eigen::Vector3f c = box_.center();
	bool x = (i % 2);
	bool y = (i % 4 > 1);
	bool z = (i >= 4);
	
	Eigen::Vector3f corigin(
		x ? c[0] : box_.origin[0],
		y ? c[1] : box_.origin[1],
		z ? c[2] : box_.origin[2]
	);
	Eigen::Vector3f cextremity(
		x ? box_.extremity[0] : c[0],
		y ? box_.extremity[1] : c[1],
		z ? box_.extremity[2] : c[2]
	);
	return bounding_box(corigin, cextremity);
}




template<typename Value, typename Weight_func>
Value field<Value, Weight_func>::get_(const Eigen::Vector3f& pos, float& confidence) const {
	ordered_weighted_samples samples = root_node_->collect_samples_around_position(pos, maximal_interpolation_samples, weight_func_);
	float total_weight = 0.0;
	Value weighted_sum = Value();
	for(const weighted_sample& ws : samples) {
		weighted_sum += ws.weight * ws.value;
		total_weight += ws.weight;
	}
	return weighted_sum / total_weight;
}


template<typename Value, typename Weight_func>
void field<Value, Weight_func>::set_(const Value& sample_value, const Eigen::Vector3f& sample_position) {
	root_node_->add(sample_value, sample_position);
}


template<typename Value, typename Weight_func>
field<Value, Weight_func>::field(const bounding_box& box, const Weight_func& func) :
	root_node_( new node(box) ),
	weight_func_(func) { }


template<typename Value, typename Weight_func>
field<Value, Weight_func>::~field() { }


template<typename Value, typename Weight_func>
void field<Value, Weight_func>::clear() {
	bounding_box box = root_node_->box();
	root_node_.reset( new node(box) );
}


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