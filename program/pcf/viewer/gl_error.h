#ifndef PCFVW_GL_ERROR_H_
#define PCFVW_GL_ERROR_H_

#include <system_error>
#include <string>

namespace pcf {

class gl_error_category : public std::error_category {
public:
	const char* name() const noexcept { return "opengl"; }
	std::string message(int condition) const noexcept { return ""; }
};


class gl_error : public std::system_error {
public:
	explicit gl_error(GLenum err) :
	std::system_error(std::error_code(err, gl_error_category())) { }
	
	explicit gl_error(const std::string& what) :
	std::system_error(std::error_code(0, gl_error_category()), what) {}
};


}

#endif