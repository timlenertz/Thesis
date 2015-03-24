#ifndef PCFVW_SHADER_PROGRAM_UNIFORM_H_
#define PCFVW_SHADER_PROGRAM_UNIFORM_H_

#include "gl.h"
#include "../pcf_core/rgb_color.h"
#include <Eigen/Eigen>

namespace pcf {

/**
Uniform variable of OpenGL shader program.
*/
class shader_program_uniform {
private:
	const GLint id_;

public:
	explicit shader_program_uniform(GLint id) : id_(id) { }
	
	shader_program_uniform& operator=(bool);
	shader_program_uniform& operator=(float);
	shader_program_uniform& operator=(double);
	shader_program_uniform& operator=(const rgb_color&);
	shader_program_uniform& operator=(const Eigen::Vector3f&);
	shader_program_uniform& operator=(const Eigen::Vector4f&);
	shader_program_uniform& operator=(const Eigen::Matrix3f&);
	shader_program_uniform& operator=(const Eigen::Matrix4f&);
};

}

#endif
