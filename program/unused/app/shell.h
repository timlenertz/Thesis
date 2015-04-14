#ifndef PCFAPP_SHELL_H_
#define PCFAPP_SHELL_H_

#include <vector>
#include <string>
#include <functional>
#include "../pcf/camera/projection_camera.h"

namespace pcf {

class program;
class viewer_window;
class viewer;

class shell {
private:
	struct program_entry {
		program* program_instance;
		std::string name;
		bool primary;
	};

	static viewer_window* viewer_win_;

	static bool main_menu_();
	static void tools_menu_();
	static void viewer_options_menu_();
	static void run_program_(program&);
	
	static std::vector<program_entry>& programs_vector_();

public:		
	static bool access_viewer(std::function<void(viewer&)>);
	static projection_camera current_viewer_camera();
	
	template<typename Program>
	static program* instanciate_program(const std::string& name, bool primary) {
		program* pr = new Program;
		programs_vector_().push_back({pr, name, primary});
		return pr;
	}

	static void main(viewer_window&);
};

}

#endif