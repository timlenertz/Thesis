#ifndef PCFVW_SCENE_OBJECT_SHADER_PROGRAM_H_
#define PCFVW_SCENE_OBJECT_SHADER_PROGRAM_H_

#include "../shader_program.h"
#include <string>

namespace pcf {

class camera;
class pose;

class scene_object_shader_program : public shader_program {
public:
	shader_program_uniform mvp_matrix;

	explicit scene_object_shader_program(const std::string&);
};

}

#endif