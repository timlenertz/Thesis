#include "scene_object_shader_program.h"
#include "../gl.h"
#include <cassert>
#include <iostream>

namespace pcf {


const std::string scene_object_shader_program::shaders_dir_ = "../pcf_viewer/shaders/";

scene_object_shader_program::scene_object_shader_program(const std::string& name) :
	super({
		shader::from_file(GL_VERTEX_SHADER, shaders_dir_ + name + ".vert"),
		shader::from_file(GL_FRAGMENT_SHADER, shaders_dir_ + name + ".frag")
	}),
	mv_matrix_uniform_(uniform_location("mv_matrix")),
	mvp_matrix_uniform_(uniform_location("mvp_matrix")),
	fog_uniform_(uniform_location("fog")),
	fog_distance_uniform_(uniform_location("fog_distance")),
	fog_color_uniform_(uniform_location("fog_color"))
{
	use();
	set_fog(false);
}



void scene_object_shader_program::set_camera(const camera& cam) {
	assert(is_current());
	
	Eigen::Matrix4f v = cam.view_transformation().matrix();
	glUniformMatrix4fv(mv_matrix_uniform_, 1, GL_FALSE, (const GLfloat*)v.data());

	Eigen::Matrix4f vp = cam.view_projection_transformation().matrix();
	glUniformMatrix4fv(mvp_matrix_uniform_, 1, GL_FALSE, (const GLfloat*)vp.data());
}


void scene_object_shader_program::set_fog(bool enable) {
	assert(is_current());
	glUniform1i(fog_uniform_, (enable ? 1 : 0));
}


void scene_object_shader_program::set_fog(bool enable, float distance, rgb_color col) {
	set_fog(enable);
	if(enable) {
		glUniform1f(fog_distance_uniform_, distance);
		float fog_color[3] = { float(col.r) / 255.0f, float(col.g) / 255.0f, float(col.b) / 255.0f };
		glUniform3fv(fog_color_uniform_, 1, fog_color);
	}
}


}