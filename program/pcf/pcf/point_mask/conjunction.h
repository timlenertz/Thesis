#ifndef PCF_POINT_MASK_CONJUNCTION_H_
#define PCF_POINT_MASK_CONJUNCTION_H_

#include <tuple>
#include <utility>

namespace pcf {

template<typename... Masks>
class point_mask_conjunction {
private:
	std::tuple<Masks...> masks_;

public:
	explicit point_mask_conjunction(Masks&&... masks);
	
	bool operator()(const point_xyz&) const;
	bool operator()(const point_full&) const;
};

}

#endif