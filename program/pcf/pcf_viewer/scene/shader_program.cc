#include "shader_program.h"
#include "../gl.h"
#include <cassert>
#include <iostream>

namespace pcf {

namespace {
	const std::string shaders_dir_ = "../pcf_viewer/shaders/";
}

scene_object_shader_program::scene_object_shader_program(const std::string& name) :
	shader_program({
		shader::from_file(GL_VERTEX_SHADER, shaders_dir_ + name + ".vert"),
		shader::from_file(GL_FRAGMENT_SHADER, shaders_dir_ + name + ".frag")
	}),
	mvp_matrix(uniform("mvp_matrix", false))
{
}


}