#include "PointCloudTransformUnit.h"
#include "../pcf/point_cloud/unorganized/unorganized_point_cloud.h"
#include <utility>

namespace pcf {

void PointCloudTransformUnit::make_unorganized_() {
	if(typeid(*object) != typeid(unorganized_point_cloud_full)) {
		point_cloud_full& old_pc = dynamic_cast<point_cloud_full&>(*object);
		auto new_pc = new unorganized_point_cloud_full( std::move(old_pc) );
		object->reset(new_pc);
	}
}


void PointCloudTransformUnit::run() {
	if(need_unorganized_) {
		make_unorganized_();
		this->run_with_point_cloud_(dynamic_cast<unorganized_point_cloud_full&>(*object_));
	} else {
		this->run_with_point_cloud_(dynamic_cast<point_cloud_full&>(*object_));
	}
}

}
