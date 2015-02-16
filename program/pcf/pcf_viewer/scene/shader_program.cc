#include "shader_program.h"
#include "../gl.h"
#include <cassert>

namespace pcf {

scene_object_shader_program::scene_object_shader_program(const std::string& vert, const std::string& frag) :
	shader_program({
		shader(GL_VERTEX_SHADER, vert),
		shader(GL_FRAGMENT_SHADER, frag)
	}),
	mvp_matrix(uniform("mvp_matrix", false))
{
}


}