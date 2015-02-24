#ifndef PCFVW_SCENE_FRUSTUM_H_
#define PCFVW_SCENE_FRUSTUM_H_

#include "object.h"
#include "../../pcf_core/geometry/frustum.h"
#include "../../pcf_core/rgb_color.h"
#include "../../pcf_core/space_object_wrapper.h"
#include "../gl.h"
#include <utility>

namespace pcf {

class scene_object_shader_program;

class scene_frustum : public scene_object {
private:
	static scene_object_shader_program* shader_program_;

	const frustum frustum_;
	rgb_color color_ = rgb_color::red;
	
	GLuint vertex_buffer_;
	GLuint vertex_array_object_;
	
protected:
	void gl_initialize_() override;
	void gl_uninitialize_() override;
	void gl_draw_() override;

public:
	scene_frustum(const scene&, space_frustum&);			
	~scene_frustum();
	
	const rgb_color& get_color() const { return color_; }
	void set_color(const rgb_color& col) { color_ = col; }
};

}

#endif