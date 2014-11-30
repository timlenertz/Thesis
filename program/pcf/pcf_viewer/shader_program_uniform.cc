#include "shader_program_uniform.h"

namespace pcf {

shader_program_uniform& shader_program_uniform::operator=(bool b) {
	glUniform1i(id_, (b ? 1 : 0));
	return *this;
}

shader_program_uniform& shader_program_uniform::operator=(float f) {
	glUniform1i(id_, f);
	return *this;
}

shader_program_uniform& shader_program_uniform::operator=(const rgb_color& col) {
	float fcol[3] = {
		float(col.r) / 255.0f,
		float(col.g) / 255.0f,
		float(col.b) / 255.0f
	};
	glUniform3fv(id_, 1, fcol);
	return *this;
}

shader_program_uniform& shader_program_uniform::operator=(const Eigen::Vector3f& v) {
	glUniform3fv(id_, 1, (const GLfloat*)v.data());
	return *this;
}

shader_program_uniform& shader_program_uniform::operator=(const Eigen::Vector4f& v) {
	glUniform4fv(id_, 1, (const GLfloat*)v.data());
	return *this;
}

shader_program_uniform& shader_program_uniform::operator=(const Eigen::Matrix3f& m) {
	glUniformMatrix3fv(id_, 1, GL_FALSE, (const GLfloat*)m.data());
	return *this;
}

shader_program_uniform& shader_program_uniform::operator=(const Eigen::Matrix4f& m) {
	glUniformMatrix4fv(id_, 1, GL_FALSE, (const GLfloat*)m.data());
	return *this;
}


}