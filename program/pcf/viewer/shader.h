#ifndef PCFVW_SHADER_H_
#define PCFVW_SHADER_H_

#include "gl.h"
#include <string>

namespace pcf {

class shader {
private:
	GLuint id_;
	
	void compile_(const std::string&);
	
public:
	static shader from_file(GLenum type, const std::string& filename);

	shader(GLenum type, const std::string& code);
	~shader();
	
	GLint get_parameter(GLenum par) const;
	std::string info_log() const;
	
	GLuint id() const { return id_; }
};
	
}

#endif
