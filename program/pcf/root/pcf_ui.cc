#ifndef __CLING__
#error Must be run from inside ROOT shell on Cling interpreter
#endif

#include "viewer.cc"

pcfui::viewer* vw;

void pcf_ui() {
	vw = new pcfui::viewer;
}