#ifndef PCFVW_SCENE_FRUSTUM_H_
#define PCFVW_SCENE_FRUSTUM_H_

#include "object.h"
#include "../../pcf/geometry/camera.h"
#include "../../pcf/rgb_color.h"
#include "../gl.h"
#include <utility>

namespace pcf {

class scene_object_shader_program;

class scene_camera : public scene_object {
private:
	static scene_object_shader_program* shader_program_;

	const camera camera_;
	rgb_color color_ = rgb_color::white;
	
	GLuint vertex_buffer_;
	GLuint vertex_array_object_;
	
protected:
	void gl_initialize_() override;
	void gl_uninitialize_() override;
	void gl_draw_() override;

public:
	scene_camera(const scene& sc, const camera& cam);			
	~scene_camera();
	
	const rgb_color& get_color() const { return color_; }
	void set_color(const rgb_color& col) { color_ = col; }
};

}

#endif