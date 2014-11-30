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

	shader_program_uniform mv_matrix_uniform_;
	shader_program_uniform mvp_matrix_uniform_;
	shader_program_uniform fog_uniform_;
	shader_program_uniform fog_distance_uniform_;
	shader_program_uniform fog_color_uniform_;

public:
	explicit scene_object_shader_program(const std::string&);
	
	void set_camera(const camera&);
	void set_fog(bool enable);
	void set_fog(bool enable, float distance, rgb_color col);
};

}

#endif