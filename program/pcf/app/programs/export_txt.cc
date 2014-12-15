#include "../program.h"
#include <string>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <fstream>

#include "../../pcf/pcf.h"

using namespace pcf;

PCF_PROGRAM(export_txt) {
	void main() {
		std::string in_filename = shell::read_line("File");
		if(in_filename.empty()) return;
		
		auto dot_pos = in_filename.find_last_of('.');
		if(dot_pos == std::string::npos) throw std::runtime_error("No filename extension.");
			
		std::unique_ptr<point_cloud_importer> imp;
		std::string ext = in_filename.substr(dot_pos + 1);
		
		std::string out_filename = in_filename.substr(0, dot_pos) + ".txt";
		
		if(ext == "ply") imp.reset(new ply_importer(in_filename));			
		else if(ext == "scan") imp.reset(new pointscan_importer(in_filename));
		else throw std::runtime_error("Unknown point cloud file extension ." + ext);

		std::cout << "Importing ." << ext << " file..." << std::endl;
		unorganized_point_cloud_xyz pc(*imp);
		imp.reset();
		
		std::cout << "Saving to " << out_filename << "..." << std::endl;
		std::ofstream out(out_filename);
		for(point_xyz& p : pc) {
			if(! p.valid()) continue;
			out << p[0] << ' ' << p[1] << ' ' << p[2] << std::endl;
		}
				
		std::cout << "Done." << std::endl;
	}
};
