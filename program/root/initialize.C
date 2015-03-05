#include <pcf/pcf_core/pcf_core.h>
#include <pcf/pcf_viewer/pcf_viewer.h>
#include "viewer_window.h"
#include <iostream>

pcfui::viewer_window vw;

#pragma link C++ global vw

void initialize() {
	std::cout << "Ready." << std::endl;
}