#include <stdexcept>

namespace pcf {

template<typename Cloud_fixed, typename Cloud_loose>
same_point_correspondences<Cloud_fixed, Cloud_loose>::same_point_correspondences
(const Cloud_fixed& cf, const Cloud_loose& cl) : fixed_(cf), loose_(cl) { }


template<typename Cloud_fixed, typename Cloud_loose> template<typename Receiver>
void same_point_correspondences<Cloud_fixed, Cloud_loose>::operator()(Receiver& rec, const Eigen::Affine3f& transformation) const {
	auto f = fixed_.cbegin();
	auto l = loose_.begin_relative_to(fixed_, transformation);

	if(mode == same_index) for(; (l < loose_.end_relative_to()) && (f < fixed_.cend()); ++f, ++l) {
		if(!f->valid() || !l->valid()) continue;
		rec << registration_correspondence(*f, *l, 1.0);
	}
	else if(mode == index_attribute) for(; l < loose_.end_relative_to() ; ++l) {
		if(!l->valid()) continue;
		std::ptrdiff_t i = l->get_index();
		const auto& pf = fixed_[i];
		if(pf.valid()) rec << registration_correspondence(pf, *l, 1.0);
	}
	else if(mode == known_transformation) for(; l < loose_.end_relative_to() ; ++l) {
		if(!l->valid()) continue;
		Eigen::Vector3f fp = real_loose_transformation * l.real_point().coordinates();
		rec << registration_correspondence(fp, *l, 1.0);
	}
}

}
