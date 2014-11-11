#ifndef PCF_REGISTRATION_CORRESPONDENCES_H_
#define PCF_REGISTRATION_CORRESPONDENCES_H_

#include <vector>
#include <type_traits>

namespace pcf {

/**
Set of fixed/loose correspondences.
Fixed and Loose must be types representing 3D coordinates, and implement operator[].
Support for std::reference_wrapper<T> types as Fixed or Loose: correspondence will
give reference to underlying type instead of reference to reference_wrapper.
*/
template<typename Fixed, typename Loose>
class registration_correspondences {
public:
	using fixed_type = Fixed;
	using loose_type = Loose;
	using fixed_reference = ref_remove_reference_wrapper<Fixed>;
	using loose_reference = ref_remove_reference_wrapper<Loose>;
	using fixed_pointer = typename std::remove_reference<fixed_reference>::type*;
	using loose_pointer = typename std::remove_reference<loose_reference>::type*;

	class correspondence {
	private:
		Fixed fixed_;
		Loose loose_;
		float weight_;
	public:
		correspondence(const Fixed& f, const Loose& l, float w = 1.0f) : fixed_(f), loose_(l), weight_(w) { }	
		fixed_reference fixed() const { return fixed_reference(fixed_); }
		loose_reference loose() const { return loose_reference(loose_); }
		float weight() const { return weight_; }
	};

private:
	std::vector<correspondence> cors_;
	
public:
	using iterator = typename std::vector<correspondence>::const_iterator; 
	
	std::size_t size() const { return cors_.size(); }
	
	iterator begin() const { return cors_.begin(); }	
	iterator end() const { return cors_.end(); }

	void clear();
	correspondence& insert(const correspondence&);
	template<typename Iterator> void insert(Iterator b, Iterator e);
	
	float mean_squared_error() const;
};

}

#include "registration_correspondences.tcc"

#endif