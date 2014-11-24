#ifndef PCF_POV_POINT_CLOUD_H_
#define PCF_POV_POINT_CLOUD_H_

#include "../tree/tree_point_cloud.h"
#include "pov_octree_traits.h"

namespace pcf {

class camera;

template<typename Point, typename Allocator = aligned_allocator<Point>>
class pov_point_cloud : public tree_point_cloud<pov_octree_traits, Point, Allocator> {
	using super = tree_point_cloud<pov_octree_traits, Point, Allocator>;
	using typename super::const_node_handle;
	
private:
	void prepare_tree_();
	std::size_t extract_points_(Point* buffer, std::size_t capacity, const const_node_handle&, std::size_t max_number) const;
		
	
public:
	template<typename Other_cloud> pov_point_cloud(Other_cloud&&, const Allocator&);
	template<typename Other_cloud> explicit pov_point_cloud(Other_cloud&&);

	std::size_t extract(Point* buffer, std::size_t capacity, const camera&) const;
};

}

#include "pov_point_cloud.tcc"

#endif
