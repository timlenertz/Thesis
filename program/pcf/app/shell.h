#ifndef PCFAPP_SHELL_H_
#define PCFAPP_SHELL_H_

#include <vector>
#include <string>

namespace pcf {

class program;

class shell {
private:
	struct program_entry {
		program* program_instance;
		std::string name;
	};

	std::vector<program_entry> programs_;

public:
	static shell& get();

	template<typename Program>
	program* instanciate_program(const std::string& name) {
		program* pr = new Program;
		programs_.push_back({pr, name});
		return pr;
	}

	void main();
};

}

#endif