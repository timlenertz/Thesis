#ifndef PCFVW_SCENE_H_
#define PCFVW_SCENE_H_

#include <set>
#include <unique_ptr>
#include <utility>
#include <pcf/geometry/camera.h>
#include "gl_object.h"
#include "scene_object.h"
#include "scene_point_cloud.h"

namespace pcf {

class scene_object_shader_program;

class scene : public gl_object {
private:
	std::set<std::unique_ptr<scene_object>> objects_;
	camera camera_;
	
protected:
	void gl_initialize_() override;
	void gl_draw_() override;

public:
	~scene();
	
	void update_camera(const camera&);
	const camera& get_camera() const { return camera_; }
		
	template<typename Cloud>
	scene_point_cloud& add_point_cloud(Cloud&& pc) {
		objects_.emplace(new scene_point_cloud(std::forward<Cloud>(pc)));
	}
};

}

#endif