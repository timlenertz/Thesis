#ifndef PCFVW_SCENE_OBJECT_SHADER_PROGRAM_H_
#define PCFVW_SCENE_OBJECT_SHADER_PROGRAM_H_

#include "../shader_program.h"
#include <string>

namespace pcf {

class camera;
class pose;

/**
Shader program for scene object.
*/
class scene_object_shader_program : public shader_program {
public:
	shader_program_uniform mvp_matrix;

	scene_object_shader_program(const std::string& vert, const std::string& frag);
};

}

#endif