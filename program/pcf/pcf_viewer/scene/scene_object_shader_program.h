#ifndef PCFVW_SCENE_OBJECT_SHADER_PROGRAM_H_
#define PCFVW_SCENE_OBJECT_SHADER_PROGRAM_H_

#include "../shader_program.h"
#include "../../pcf/rgb_color.h"
#include "../../pcf/geometry/camera.h"
#include "../../pcf/geometry/pose.h"
#include <string>

namespace pcf {

class camera;
class pose;

class scene_object_shader_program : public shader_program {
	using super = shader_program;

private:
	static const std::string shaders_dir_;

	GLint mv_matrix_uniform_;
	GLint mvp_matrix_uniform_;
	GLint fog_uniform_;
	GLint fog_distance_uniform_;
	GLint fog_color_uniform_;

public:
	explicit scene_object_shader_program(const std::string&);
	
	void set_camera(const camera&);
	void set_fog(bool enable);
	void set_fog(bool enable, float distance, rgb_color col);
};

}

#endif