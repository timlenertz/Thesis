#ifndef PCFVW_SCENE_H_
#define PCFVW_SCENE_H_

#include <set>
#include <unique_ptr>
#include <utility>
#include <pcf/geometry/camera.h>
#include <pcf/geometry/angle.h>
#include "gl_object.h"
#include "scene_object.h"
#include "scene_point_cloud.h"

namespace pcf {

class scene_object_shader_program;


/**
Scene composed of camera and scene objects.
Recursively calls its objects' rendering functions. Scene alone handles the complete OpenGL rendering operation. Viewer may provide additional UI elements and handles control of movement through scene.
*/
class scene : public gl_object {
private:
	std::set<std::unique_ptr<scene_object>> objects_;
	camera camera_;
		
protected:
	void gl_initialize_() override;
	void gl_draw_() override;

public:
	scene(std::size_t view_w, std::size_t view_h, angle fov_x);
	~scene();
	
	void set_camera_parameters(std::size_t view_w, std::size_t view_h, angle fov_x);
	
	void set_camera_pose(const pose&);
	const pose& get_camera_pose() const;
	
	camera& get_camera() { return camera_; }
	const camera& get_camera() const { return camera_; }
		
	template<typename Cloud>
	scene_point_cloud& add_point_cloud(Cloud&& pc) {
		objects_.emplace(new scene_point_cloud(std::forward<Cloud>(pc)));
	}
};

}

#endif