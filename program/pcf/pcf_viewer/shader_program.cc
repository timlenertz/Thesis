#include "shader_program.h"
#include "gl_error.h"

#include <memory>

namespace pcf {
	
void shader_program::link_() {
	glLinkProgram(id_);
	if(! get_parameter(GL_LINK_STATUS))
		throw gl_error(std::string("Failed to link program. Info log: " + info_log()));
}

std::string shader_program::info_log() const {
	GLsizei maxlen = get_parameter(GL_INFO_LOG_LENGTH);
	GLsizei len = 0;
	std::unique_ptr<GLchar[]> log(new GLchar[maxlen]);
	glGetProgramInfoLog(id_, maxlen, &len, log.get());
	if(len == 0) return "";
	return std::string((char*)log.get(), len);
}

GLint shader_program::get_parameter(GLenum par) const {
	GLint res;
	glGetProgramiv(id_, par, &res);
	return res;
}


shader_program_uniform shader_program::uniform(const std::string& name, bool cache) {
	if(cache) {
		auto it = uniform_locations_.find(name);
		if(it != uniform_locations_.end()) return shader_program_uniform(it->second);
	}
	GLint id = glGetUniformLocation(id_, name.c_str());
	if(cache) uniform_locations_[name] = id;
	return shader_program_uniform(id);
}


void shader_program::use() const {
	glUseProgram(id_);
}

bool shader_program::is_current() const {
	GLint cur;
	glGetIntegerv(GL_CURRENT_PROGRAM, &cur);
	return (cur == id_);
}

shader_program::shader_program(std::initializer_list<shader> shaders) {
	id_ = glCreateProgram();
	for(const shader& sh : shaders) glAttachShader(id_, sh.id());
	link_();
}

shader_program::~shader_program() {
	glDeleteProgram(id_);
}


}