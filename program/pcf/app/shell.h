#ifndef PCFAPP_SHELL_H_
#define PCFAPP_SHELL_H_

#include <vector>
#include <string>

namespace pcf {

class program;
class viewer_window;

class shell {
private:
	struct program_entry {
		program* program_instance;
		std::string name;
	};

	static viewer_window* viewer_win_;

	static void run_program_(program&);
	
	static std::vector<program_entry>& programs_vector_();

public:
	template<class T> static T read_from_string(const std::string& str);
	template<class T> static T read_from_string(const std::string& str, const T& def);
	template<class T> static T read_from_input(const std::string& prompt);
	template<class T> static T read_from_input(const std::string& prompt, const T& def);
	static std::string read_line(const std::string& prompt, const std::string& def = std::string(""));
	
	static void set_viewer_window(viewer_window&);
	static viewer_window& get_viewer_window();
	static bool has_viewer_window();
	
	template<typename Program>
	static program* instanciate_program(const std::string& name);

	static void main();
};

}

#include "shell.tcc"

#endif