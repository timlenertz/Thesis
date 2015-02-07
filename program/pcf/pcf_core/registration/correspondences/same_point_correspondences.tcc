#include <stdexcept>

namespace pcf {

template<typename Cloud_fixed, typename Cloud_loose>
same_point_correspondences<Cloud_fixed, Cloud_loose>::same_point_correspondences
(const Cloud_fixed& cf, const Cloud_loose& cl) : fixed_(cf), loose_(cl) {
	if(fixed_.size() != loose_.size())
		throw std::invalid_argument("Fixed and loose clouds need to have same size.");
}


template<typename Cloud_fixed, typename Cloud_loose> template<typename Receiver>
void same_point_correspondences<Cloud_fixed, Cloud_loose>::operator()(Receiver& rec) {
	auto f = fixed_.cbegin();
	auto l = loose_.begin_transform();
	for(; l < loose_.end_transform(); ++f, ++l) {
		if(!f->valid() || !l->valid()) continue;
		rec << correspondence_type(*f, *l, l.real_point(), 1.0);
	}
}

}
