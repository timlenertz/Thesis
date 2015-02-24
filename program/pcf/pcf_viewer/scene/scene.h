#ifndef PCFVW_SCENE_H_
#define PCFVW_SCENE_H_

#include <set>
#include <vector>
#include <memory>
#include <utility>
#include <stdexcept>
#include <type_traits>
#include "../../pcf_core/camera/projection_image_camera.h"
#include "../../pcf_core/geometry/angle.h"
#include "../../pcf_core/point_cloud/point_cloud.h"
#include "../../pcf_core/rgb_color.h"
#include "../gl_object.h"
#include "object.h"
#include "object_holder.h"

namespace pcf {

class scene_object_shader_program;

/**
Scene composed of camera and scene objects.
Recursively calls its objects' rendering functions. Scene alone handles the complete OpenGL rendering operation. Viewer may provide additional UI elements and handles control of movement through scene.
*/
class scene : public gl_object {
private:
	std::set<scene_object*> objects_;
	std::vector<std::unique_ptr<scene_object_holder_base>> holders_;
	projection_image_camera camera_;
	rgb_color background_color_ = rgb_color::black;
	
	bool should_reset_viewport_ = true;
	
	void notify_camera_update_();

	template<typename Scene_object, typename... Args>
	void add_with_holder_(Args&...);
	
	scene(const scene&) = delete;

protected:
	void gl_initialize_() override;
	void gl_draw_() override;

public:
	scene(std::size_t view_w, std::size_t view_h, angle fov_x);
	~scene();
			
	const projection_image_camera& get_camera() const;
	void set_camera(const projection_image_camera&);
	
	const pose& get_camera_pose() const;
	void set_camera_pose(const pose&);
	void set_camera_image_size(std::size_t w, std::size_t h);
		
	void clear();
	void add_object(scene_object&);
	void erase_object(scene_object&);
	
	void add(point_cloud_xyz&);
	void add(point_cloud_full&);
};



template<typename Scene_object, typename... Args>
void scene::add_with_holder_(Args&... args) {
	auto holder = new scene_object_holder<Scene_object, Args...>(*this, args...);
	holders_.emplace_back(holder);
}

}

#endif