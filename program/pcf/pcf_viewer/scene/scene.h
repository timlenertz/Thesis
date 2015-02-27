#ifndef PCFVW_SCENE_H_
#define PCFVW_SCENE_H_

#include <set>
#include <vector>
#include <memory>
#include <utility>
#include <stdexcept>
#include <type_traits>
#include <string>
#include "../../pcf_core/camera/projection_image_camera.h"
#include "../../pcf_core/geometry/angle.h"
#include "../../pcf_core/point_cloud/point_cloud.h"
#include "../../pcf_core/rgb_color.h"
#include "../../pcf_core/space_object_observer.h"
#include "../gl_object.h"
#include "object.h"
#include "object_holder.h"

#include <iostream>

namespace pcf {

class scene_object_shader_program;

/**
Scene composed of camera and scene objects.
Recursively calls its objects' rendering functions. Scene alone handles the complete OpenGL rendering operation. Viewer may provide additional UI elements and handles control of movement through scene.
*/
class scene : public gl_object, private space_object_observer {
private:
	std::set<scene_object*> objects_;
	std::vector<std::unique_ptr<scene_object_holder_base>> holders_;
	projection_image_camera camera_;
	rgb_color background_color_ = rgb_color::black;
	bool should_reset_viewport_ = true;
	
	void notify_camera_update_();

	template<typename Scene_object, typename... Args>
	void add_with_holder_(Args&...);
	
	const scene_object_holder_base& holder_with_name_(const std::string&) const;
	scene_object_holder_base& holder_with_name_(const std::string&);
	bool has_holder_with_name_(const std::string&) const;
	
	// Handlers for when camera got moved or updated...
	void pose_was_updated_() override;
	void object_was_updated_() override;
	
	scene(const scene&) = delete;

protected:
	void gl_initialize_() override;
	void gl_draw_() override;

public:
	scene(std::size_t view_w, std::size_t view_h, angle fov_x);
	~scene();
			
	projection_image_camera& get_camera() { return camera_; }
	const projection_image_camera& get_camera() const { return camera_; }
		
	void add_object(scene_object&);
	void erase_object(scene_object&);
	
	scene_object& scene_object_with_name(const std::string&);
	const scene_object& scene_object_with_name(const std::string&) const;
	space_object& space_object_with_name(const std::string&);
	const space_object& space_object_with_name(const std::string&) const;
	bool has_object_with_name(const std::string&) const;

	void clear();	
	void add(point_cloud_xyz&);
	void add(point_cloud_full&);
	
	template<typename Object_1, typename Object_2, typename... Other_objects>
	void add(Object_1& obj1, Object_2& obj2, Other_objects&... others) {
		add(obj1);
		add(obj2);
		add(others...);
	}
	void add() { }
};



template<typename Scene_object, typename... Args>
void scene::add_with_holder_(Args&... args) {
	auto holder = new scene_object_holder<Scene_object, Args...>(*this, args...);
	holders_.emplace_back(holder);
}

}

#endif