#include <stdexcept>

namespace pcf {

template<typename Cloud_fixed, typename Cloud_loose>
same_point_correspondences<Cloud_fixed, Cloud_loose>::same_point_correspondences
(const Cloud_fixed& cf, const Cloud_loose& cl) : fixed_(cf), loose_(cl) { }


template<typename Cloud_fixed, typename Cloud_loose> template<typename Receiver>
void same_point_correspondences<Cloud_fixed, Cloud_loose>::operator()(Receiver& rec, const Eigen::Affine3f& transformation) {
	auto f = fixed_.cbegin();
	auto l = loose_.begin_relative_to(fixed_, transformation);
	
	for(; (l < loose_.end_relative_to()) && (f < fixed_.cend()); ++f, ++l) {
		if(!f->valid() || !l->valid()) continue;
		rec << correspondence_type(*f, *l, l.real_point(), 1.0);
	}
}

}
