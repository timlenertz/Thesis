#ifndef PCFVW_SCENE_BOUNDING_BOX_H_
#define PCFVW_SCENE_BOUNDING_BOX_H_

#include "object.h"
#include "../../pcf_core/geometry/bounding_box.h"
#include "../../pcf_core/rgb_color.h"
#include "../gl.h"
#include <utility>

namespace pcf {

class scene_object_shader_program;

class scene_bounding_box : public scene_object {
private:
	static scene_object_shader_program* shader_program_;

	const bounding_box box_;
	rgb_color color_ = rgb_color::white;
	
	GLuint vertex_buffer_;
	GLuint vertex_array_object_;
	
protected:
	void gl_initialize_() override;
	void gl_uninitialize_() override;
	void gl_draw_() override;

public:
	scene_bounding_box(const scene& sc, const bounding_box& box);			
	~scene_bounding_box();
	
	const rgb_color& get_color() const { return color_; }
	void set_color(const rgb_color& col) { color_ = col; }
};

}

#endif