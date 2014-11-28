#include "shader.h"

#include <memory>
#include <string>
#include <fstream>
#include "gl.h"
#include "gl_error.h"

namespace pcf {

shader shader::from_file(GLenum type, const std::string& filename) {
	std::string line, code;
	std::ifstream str(filename);
	while(std::getline(str, line)) code += line + '\n';
	str.close();
	return shader(type, code);
}

void shader::compile_(const std::string& code) {
	const GLchar* code_str = code.c_str();
	const GLint code_len = code.size();
	
	glShaderSource(id_, 1, &code_str, &code_len);
	glCompileShader(id_);	
	if(! get_parameter(GL_COMPILE_STATUS))
		throw gl_error(std::string("Failed to compile shader. Info log: \n" + info_log()));
}

shader::shader(GLenum type, const std::string& code) {	
	id_ = glCreateShader(type);
	compile_(code);
}

GLint shader::get_parameter(GLenum par) const {
	GLint res;
	glGetShaderiv(id_, par, &res);
	return res;
}

shader::~shader() {
	glDeleteShader(id_);
}

std::string shader::info_log() const {
	GLsizei maxlen = get_parameter(GL_INFO_LOG_LENGTH);
	GLsizei len = 0;
	std::unique_ptr<GLchar[]> log(new GLchar[maxlen]);
	glGetShaderInfoLog(id_, maxlen, &len, log.get());
	if(len == 0) return "";
	return std::string((char*)log.get(), len);
}

}
