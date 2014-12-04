#include "shader_program.h"
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
	mvp_matrix_uniform_(uniform("mvp_matrix")),
	fog_uniform_(uniform("fog")),
	fog_distance_uniform_(uniform("fog_distance")),
	fog_color_uniform_(uniform("fog_color"))
{
	use();
	set_fog(false);
}



void scene_object_shader_program::set_mvp(const camera& cam, const pose& ps) {
	assert(is_current());
	Eigen::Matrix4f mvp = ps.view_transformation().matrix() * cam.view_projection_transformation().matrix();;
	mvp_matrix_uniform_ = mvp;
}


void scene_object_shader_program::set_fog(bool enable) {
	assert(is_current());
	fog_uniform_ = enable;
}


void scene_object_shader_program::set_fog(bool enable, float distance, rgb_color col) {
	set_fog(enable);
	if(enable) {
		fog_distance_uniform_ = distance;
		fog_color_uniform_ = col;
	}
}


}