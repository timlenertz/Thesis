#ifndef PCF_REGISTRATION_CORRESPONDENCES_H_
#define PCF_REGISTRATION_CORRESPONDENCES_H_

#include <vector>

namespace pcf {

/**
Set of fixed/loose correspondences.
Support for std::reference_wrapper<T> types as Fixed or Loose: correspondence will
give reference to underlying type instead of reference to reference_wrapper.
*/
template<typename Fixed, typename Loose>
class registration_correspondences {
public:
	using fixed_type = Fixed;
	using loose_type = Loose;
	using fixed_reference = ref_remove_reference_wrapper<Fixed&>;
	using loose_reference = ref_remove_reference_wrapper<Loose&>;

	class correspondence {
	private:
		Fixed fixed_;
		Loose loose_;
	
	public:
		correspondence(const Fixed& f, const Loose& l) :
			fixed_(f), loose_(l) { }
	
		fixed_reference fixed() const { return fixed_reference(fixed_); }
		loose_reference loose() const { return loose_reference(loose_); }
	};

private:
	std::vector<correspondence> cors_;
	
public:
	using iterator = std::vector<correspondence>::iterator; 

	void clear() { cors_.clear(); }
	std::size_t size() const { return cors_.size(); }
	
	iterator begin() { cors_.begin(); }	
	iterator end() { cors_.end(); }
};


}

#endif