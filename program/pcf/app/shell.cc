#include "shell.h"
#include "program.h"
#include "viewer_window.h"
#include "../pcf_viewer/viewer.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <memory>

namespace pcf {

viewer_window* shell::viewer_win_ = nullptr;

std::vector<shell::program_entry>& shell::programs_vector_() {
	using vec = std::vector<shell::program_entry>;
	static vec* programs_ = nullptr;
	if(! programs_) programs_ = new vec;
	return *programs_;
}


void shell::viewer_options_menu_() {
	std::vector<std::string> choices = {
		"Movement speed: " + std::to_string(get_viewer_window().get_movement_speed()),
		"Clear scene objects",
		"Reset camera",
		"Camera pose"
	};
	
	unsigned choice = read_choice("Option", choices);
	if(choice == 0) {
		float speed = read_from_input("New movement speed", get_viewer_window().get_movement_speed());
		get_viewer_window().set_movement_speed(speed);
	} else if(choice == 1) {
		access_viewer([](viewer& vw) {
			vw->clear();
		});
	} else if(choice == 2) {
		access_viewer([](viewer& vw) {
			vw->set_camera_pose(pose());
		});
	} else if(choice == 3) {
		access_viewer([](viewer& vw) {
			const pose& ps = vw->get_camera_pose();
			std::cout << "View matrix:" << std::endl;
			std::cout << ps.view_transformation().matrix() << std::endl;
			std::cout << "Position:" << std::endl;
			std::cout << ps.position << std::endl;
		});
	}
}



void shell::run_program_(program& p) {
	if(has_viewer_window()) get_viewer_window().access_viewer([](viewer& vw) {
		vw->clear();
	});
	
	try {
		p.main();
	} catch(const std::exception& ex) {
		std::cout << "Exception: " << ex.what() << std::endl;
	} catch(...) {
		std::cout << "Unknown exception" << std::endl;
	}
}


void shell::tools_menu_() {
	std::vector<std::string> choices = {
		"Exit"
	};
	std::vector<program*> programs;
	for(const program_entry& p : programs_vector_()) if(! p.primary) {
		choices.push_back(p.name);
		programs.push_back(p.program_instance);
	}
		
	unsigned choice = read_choice("Tool Program", choices);
	if(choice == 0) return;
	else run_program_(*programs[choice - 1]);
}


bool shell::main_menu_() {
	std::vector<std::string> choices = {
		"Exit",
		"Viewer options",
		"Tools"
	};
	std::vector<program*> programs;
	for(const program_entry& p : programs_vector_()) if(p.primary) {
		choices.push_back(p.name);
		programs.push_back(p.program_instance);
	}
		
	
	unsigned choice = read_choice("Program", choices);
	if(choice == 0) return false;
	else if(choice == 1) viewer_options_menu_();
	else if(choice == 2) tools_menu_();
	else run_program_(*programs[choice - 3]);
	
	return true;
}


void shell::main() {
	bool run = true;
	while(run) run = main_menu_();
}


std::string shell::read_line(const std::string& prompt, const std::string& def) {
	std::cout << prompt << ": ";
	std::string line;
	std::getline(std::cin, line);
	if(line.empty()) return def;
	else return line;
}


int shell::read_choice(const std::string& prompt, const choices& chc) {
	std::cout << prompt << ':' << std::endl;
	int i = 1;
	for(const std::string& choice : chc)
		std::cout << " (" << i++ << ") " << choice << std::endl;

	int selection = 0;
	while(selection == 0 || selection > chc.size())
		selection = read_from_input("Choose", 0);
	
	return selection - 1;
}



void shell::set_viewer_window(viewer_window& win) {
	viewer_win_ = &win;
}


viewer_window& shell::get_viewer_window() {
	if(has_viewer_window()) return *viewer_win_;
	else throw std::logic_error("No viewer window.");
}


bool shell::has_viewer_window() {
	return (viewer_win_ != nullptr) && ! viewer_win_->was_closed();
}



bool shell::access_viewer(std::function<void(viewer&)> cb) {
	if(! has_viewer_window()) return false;
	get_viewer_window().access_viewer(cb);
	return true;
}


projection_camera shell::current_viewer_camera() {
	std::unique_ptr<projection_camera> cam = nullptr;
	access_viewer([&cam](viewer& vw) {
		const projection_camera& c = vw->get_camera();
		cam.reset(new projection_camera(c));
	});
	if(cam) return *cam;
	else throw std::runtime_error("Could not get current viewer camera.");
}

}