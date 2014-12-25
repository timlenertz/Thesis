#include "../program.h"
#include <iostream>

#include "../lib.h"

using namespace pcf;

PCF_PROGRAM(icp_lab, true) {
private:
	unorganized_point_cloud_stack_full* fixed_;
	unorganized_point_cloud_stack_full* loose_;
	
	scene_point_cloud* scene_fixed_;
	scene_point_cloud* scene_loose_;
	
	using same_point_cor = same_point_correspondences<
		unorganized_point_cloud_stack_full,
		unorganized_point_cloud_stack_full
	>;
	
	same_point_cor* same_correspondences_ = nullptr;

	void destroy() {
		delete fixed_;
		delete loose_;
	}

	bool load_point_clouds_() {
		std::string fixed_file_name = read_line("Fixed");
		if(fixed_file_name.empty()) return false;
		std::string loose_file_name = read_line("Loose", fixed_file_name);
		
		fixed_ = new unorganized_point_cloud_stack_full(load_point_cloud(fixed_file_name));
		loose_ = new unorganized_point_cloud_stack_full(load_point_cloud(loose_file_name));
		
		if(loose_file_name.empty()) {
			print("Using same point cloud as fixed and loose");
			same_correspondences_ = new same_point_cor(*fixed_, *loose_);
		}
		
		print("Adding point clouds to scene...");
		shell::access_viewer([&](viewer& vw) {
			vw->clear();
			auto& sc_fx = vw->add_point_cloud(*fixed_);
			scene_fixed_ = &sc_fx;
			auto& sc_ls = vw->add_point_cloud(*loose_);
			scene_loose_ = &sc_ls;
		});
		
		return true;
	}
	
	void modify_menu_(unorganized_point_cloud_full* pc, scene_point_cloud* spc) {
		switch(read_choice("Point Cloud Action", {
			"Exit",
			"Colorize",
			"Move",
			"Crop Box",
			"Downsample",
			"Displace",
			"Add Noise"
		})) {
		default:
			break;
		}
	}
	
	bool main_menu_() {
		switch(read_choice("Action", {
			"Exit",
			"Modify Fixed",
			"Modify Loose",
			"Run ICP"
		})) {
		case 0:
			return false;
		case 1:
			print("Fixed Point Cloud");
			modify_menu_(fixed_, scene_fixed_);
			break;
		case 2:
			print("Loose Point Cloud");
			modify_menu_(loose_, scene_loose_);
			break;
		case 3:
		default:
			break;
		}
		return true;
	}


public:
	void main() {
		bool loaded = load_point_clouds_();
		if(! loaded) return;
		
		bool run = true;
		while(run)
			try {
				run = main_menu_();
			} catch(const std::exception& ex) {
				print("Unhandled exception: ", ex.what());
			}
		
		destroy();
	}
};
