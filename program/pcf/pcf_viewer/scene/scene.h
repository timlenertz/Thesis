#ifndef PCFVW_SCENE_H_
#define PCFVW_SCENE_H_

#include <set>
#include <memory>
#include <utility>
#include <stdexcept>
#include "../../pcf/geometry/projection_camera.h"
#include "../../pcf/geometry/angle.h"
#include "../gl_object.h"
#include "object.h"
#include "point_cloud.h"

namespace pcf {

class scene_object_shader_program;
class scene_bounding_box;

/**
Scene composed of camera and scene objects.
Recursively calls its objects' rendering functions. Scene alone handles the complete OpenGL rendering operation. Viewer may provide additional UI elements and handles control of movement through scene.
*/
class scene : public gl_object {
private:
	std::set<std::unique_ptr<scene_object>> objects_;
	projection_camera camera_;
	bool should_reset_viewport_ = true;
		
protected:
	void gl_initialize_() override;
	void gl_draw_() override;

public:
	scene(std::size_t view_w, std::size_t view_h, angle fov_x);
	~scene();
			
	const projection_camera& get_camera() const;
	void set_camera(const projection_camera&);
	
	const pose& get_camera_pose() const;
	void set_camera_pose(const pose&);
	void set_camera_image_size(std::size_t w, std::size_t h);
	void set_camera_field_of_view(angle);
	
	void update_camera(const projection_camera&);
	
	void clear();
	
	template<typename Cloud>
	scene_point_cloud& add_point_cloud(Cloud&& pc) {
		scene_point_cloud* spc = new scene_point_cloud(*this, std::forward<Cloud>(pc));
		objects_.emplace(spc);
		return *spc;
	}
	
	scene_bounding_box& add_bounding_box(const bounding_box&);
};

}

#endif