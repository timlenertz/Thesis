#ifdef __APPLE__

#include "../program.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <glob.h>
#include <dirent.h> // TODO make cross-plaform
#include <vector>

#include "../lib.h"

using namespace pcf;

PCF_PROGRAM(export_range_point_cloud_images, false) {
	void main() {
		std::vector<std::string> files;
	
		std::string townhall_scans_dir = "../../../townhall/";
		std::string range_output_dir = "../../../townhall/range/";
		std::string color_output_dir = "../../../townhall/color/";
	
		DIR* dir = opendir(townhall_scans_dir.c_str());
		while(dirent* ent = readdir(dir)) {
			const char* name = ent->d_name;
			files.emplace_back(name);
		}
		closedir(dir);
		
		
		#pragma omp parallel for
		for(std::ptrdiff_t i = 0; i < files.size(); ++i) {
			const std::string& name = files[i];
		
			std::string filename = townhall_scans_dir + name;
			if(file_name_extension(filename) != "scan") continue;
			
			std::string range_filename = range_output_dir + change_file_name_extension(name, "png");
			std::string color_filename = color_output_dir + change_file_name_extension(name, "png");
			
			pointscan_importer imp(filename);
			range_point_cloud_full pc(imp);

			std::cout << "Scan " << name << "..." << std::endl;
			{
				std::cout << "Generating color image..." << std::endl;
				color_image ci = pc.to_color_image(rgb_color::black);
				ci.flip(true, false);
		
				std::cout << "Exporting color image..." << std::endl;
				ci.export_to_image_file(color_filename);
			}

			{
				std::cout << "Generating range image..." << std::endl;
				range_image ri = pc.to_range_image();
				ri.flip(true, false);
		
				std::cout << "Exporting range image..." << std::endl;
				ri.export_visualization_to_image_file(range_filename);
			}

		}		

		
		std::cout << "Done." << std::endl;
	}
};

#endif