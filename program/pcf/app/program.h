#ifndef PCFAPP_PROGRAM_H_
#define PCFAPP_PROGRAM_H_

#include <string>
#include "shell.h"

#define PCF_PROGRAM(Name) \
	class program_ ## Name; \
	const static program* program_instance_ ## Name = shell::get().instanciate_program<program_ ## Name>(#Name); \
	class program_ ## Name : public program

namespace pcf {

class program {	
public:
	virtual ~program() { }
	
	virtual void main() = 0;
};

}

#endif
