#include "pcf_core/pcf_core.h"
#include "pcf_viewer/pcf_viewer.h"
#include "viewer_window.h"

pcfui::viewer_window vw;

#pragma link C++ global vw

void initialize() {
	using namespace pcf;
	ply_importer imp("../data/dragon.ply");
	unorganized_point_cloud_xyz pc(imp);
	vw->add_point_cloud(pc);
}