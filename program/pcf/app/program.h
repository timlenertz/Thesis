#ifndef PCFAPP_PROGRAM_H_
#define PCFAPP_PROGRAM_H_

#include <string>
#include "shell.h"
#include "viewer_window.h"

#define PCF_PROGRAM(Name) \
	struct program_ ## Name; \
	const static program* program_instance_ ## Name = shell::instanciate_program<program_ ## Name>(#Name); \
	struct program_ ## Name : public program


namespace pcf {

class program {
protected:
	template<typename Callback>
	bool access_viewer_(const Callback& cb) {
		if(! shell::has_viewer_window()) return false;
		shell::get_viewer_window().access_viewer(cb);
		return true;
	}
	
public:
	virtual ~program() { }
	
	virtual void main() = 0;
};

}

#endif
