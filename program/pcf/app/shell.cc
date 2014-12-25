#include "shell.h"
#include "program.h"
#include "viewer_window.h"
#include "../pcf_viewer/viewer.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <memory>
#include "tui.h"

namespace pcf {

viewer_window* shell::viewer_win_ = nullptr;

std::vector<shell::program_entry>& shell::programs_vector_() {
	using vec = std::vector<shell::program_entry>;
	static vec* programs_ = nullptr;
	if(! programs_) programs_ = new vec;
	return *programs_;
}


void shell::viewer_options_menu_() {
	choices cs = {
		"Movement speed: " + std::to_string(viewer_win_->get_movement_speed()),
		"Clear scene objects",
		"Reset camera",
		"Camera pose"
	};
	
	unsigned choice = read_choice("Option", cs);
	if(choice == 0) {
		float speed = read_from_input("New movement speed", viewer_win_->get_movement_speed());
		viewer_win_->set_movement_speed(speed);
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
	access_viewer([](viewer& vw) {
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
	choices cs = {
		"Exit"
	};
	std::vector<program*> programs;
	for(const program_entry& p : programs_vector_()) if(! p.primary) {
		cs.push_back(p.name);
		programs.push_back(p.program_instance);
	}
		
	unsigned choice = read_choice("Tool Program", cs);
	if(choice == 0) return;
	else run_program_(*programs[choice - 1]);
}


bool shell::main_menu_() {
	choices cs = {
		"Exit",
		"Viewer options",
		"Tools"
	};
	std::vector<program*> programs;
	for(const program_entry& p : programs_vector_()) if(p.primary) {
		cs.push_back(p.name);
		programs.push_back(p.program_instance);
	}
		
	
	unsigned choice = read_choice("Program", cs);
	if(choice == 0) return false;
	else if(choice == 1) viewer_options_menu_();
	else if(choice == 2) tools_menu_();
	else run_program_(*programs[choice - 3]);
	
	return true;
}


void shell::main(viewer_window& win) {
	viewer_win_ = &win;
	bool run = true;
	while(run) run = main_menu_();
	win.signal_close();
}


bool shell::access_viewer(std::function<void(viewer&)> cb) {
	viewer_win_->access_viewer(cb);
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