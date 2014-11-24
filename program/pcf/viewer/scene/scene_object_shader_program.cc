#include "scene_object_shader_program.h"

namespace pcf {

const std::string scene_object_shader_program::shaders_dir_ = "./shaders/";


scene_object_shader_program::scene_object_shader_program(const std::string& name) :
super({
	shader::from_file(shaders_dir_ + name + ".vert"),
	shader::from_file(shaders_dir_ + name + ".frag")
}) {
	mv_matrix_uniform_ = uniform_location("mv_matrix");
	mvp_matrix_uniform_ = uniform_location("mvp_matrix");
	fog_uniform_ = uniform_location("fog");
	fog_distance_uniform_ = uniform_location("fog_distance");
	fog_color_uniform_ = uniform_location("fog_color");
}

void scene_object_shader_program::set_camera(const camera& cam) {
	
}

void scene_object_shader_program::set_fog(bool enable) {
	glUniform1ui(fog_uniform_, (enable ? 1 : 0));
}

void scene_object_shader_program::set_fog(bool enable, float distance, rgb_color col) {
	if(enable) {
		glUniform1f(fog_distance_uniform_, distance);
		float fog_color[3] = { float(col.r) / 255.0f, float(col.g) / 255.0f, float(col.b) / 255.0f };
		glUniform3fv(fog_color_uniform_, 1, fog_color);
	}
	glUniform1ui(fog_uniform_, (enable ? 1 : 0));
}


}