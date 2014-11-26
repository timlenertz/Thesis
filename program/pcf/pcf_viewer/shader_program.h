#ifndef PCFVW_SHADER_PROGRAM_H_
#define PCFVW_SHADER_PROGRAM_H_

#include "gl.h"
#include "shader.h"

#include <string>
#include <initializer_list>

namespace pcf {

/**
OpenGL program composed of shaders.
*/
class shader_program {
private:
	GLuint id_;
	
	void link_();
	
public:
	program(std::initializer_list<shader> shaders);
	~program();
	
	GLint get_parameter(GLenum par) const;
	std::string info_log() const; 

	GLuint id() const { return id_; }
	GLint uniform_location(const std::string& name);

	void use() const;
	bool is_current() const;
};

}

#endif
