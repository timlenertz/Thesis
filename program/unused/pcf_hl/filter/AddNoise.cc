#include "AddNoise.h"
#include "../../pcf/point_cloud/unorganized/unorganized_point_cloud.h"

namespace pcf {

AddNoise::AddNoise() : PointCloudTransformUnit(true) { }

void AddNoise::run_with_point_cloud_(unorganized_point_cloud_full& pc) {
	bounding_box box = pc.box();
	pc.add_random_noise_in_box(amount, box);
}

}