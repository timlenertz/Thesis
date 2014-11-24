#ifndef PCFVW_SCENE_POINT_CLOUD_H_
#define PCFVW_SCENE_POINT_CLOUD_H_

#include "scene_object.h"
#include <pcf/point_cloud/pov/pov_point_cloud.h>

namespace pcf {

class scene_point_cloud : public scene_object {
private:
	pov_point_cloud_full cloud_;

public:
	template<typename Cloud>
	explicit scene_point_cloud(Cloud&&);
	
	void draw() const override;
};

}

#endif