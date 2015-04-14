#ifndef PCFHL_ADD_NOISE_H_
#define PCFHL_ADD_NOISE_H_

#include "../PointCloudTransformUnit.h"

namespace pcf {

class AddNoise : public PointCloudTransformUnit {
public:
	std::size_t amount = 1000000;

	AddNoise();

	void run_with_point_cloud_(unorganized_point_cloud_full&) override;
};


}

#endif