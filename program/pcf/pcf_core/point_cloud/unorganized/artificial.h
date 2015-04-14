#ifndef PCF_ARTIFICIAL_POINT_CLOUD_H_
#define PCF_ARTIFICIAL_POINT_CLOUD_H_

#include "unorganized_point_cloud.h"
#include "../range/range_point_cloud.h"
#include "../../geometry/bounding_box.h"
#include <vector>
#include <Eigen/Eigen>

namespace pcf {

/// Create point cloud of a sphere centered at origin.
/// The points are randomly distributed on the surface at a uniform density.
unorganized_point_cloud_full make_sphere_point_cloud(float radius, std::size_t number_of_points);

unorganized_point_cloud_full make_sphere_point_cloud_with_density(float radius, float density);

class relief {
private:
	struct engraving {
		float x;
		float y;
		float radius;
		float depth;
		bool out;
	};
	std::vector<engraving> engravings_;
	float width_;

public:
	explicit relief(float width, int seed = 0);
	
	range_point_cloud_full make_point_cloud(float density) const;
	point_full at(float x, float y) const;
};

inline range_point_cloud_full make_relief_point_cloud(float width, float density, int seed = 0) {
	relief rel(width, seed);
	return rel.make_point_cloud(density);
}

}

#endif