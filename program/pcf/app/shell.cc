#include "shell.h"
#include "program.h"
#include "viewer_window.h"
#include "../pcf_viewer/viewer.h"
#include <iostream>
#include <stdexcept>
#include <memory>

namespace pcf {

viewer_window* shell::viewer_win_ = nullptr;

std::vector<shell::program_entry>& shell::programs_vector_() {
	using vec = std::vector<shell::program_entry>;
	static vec* programs_ = nullptr;
	if(! programs_) programs_ = new vec;
	return *programs_;
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


void shell::main() {
	bool stop = false;
	while(! stop) {	
		unsigned i = 1;
		for(const program_entry& p : programs_vector_()) {
			std::cout << '(' << i++ << ") " << p.name << std::endl;
		}
		
		while(i < 1 || i > programs_vector_().size()) i = read_from_input("Program", 0);
		run_program_(*programs_vector_()[i - 1].program_instance);
	}
}


std::string shell::read_line(const std::string& prompt, const std::string& def) {
	std::cout << prompt << ": ";
	std::string line;
	std::getline(std::cin, line);
	if(line.empty()) return def;
	else return line;
}


int shell::read_choice(const std::string& prompt, const std::vector<std::string>& choices) {
	std::cout << prompt << ':' << std::endl;
	int i = 1;
	for(const std::string& choice : choices)
		std::cout << " (" << i++ << ") " << choice << std::endl;

	int selection = 0;
	while(selection == 0 || selection > choices.size())
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