#include "../program.h"
#include <string>
#include <iostream>
#include <stdexcept>

#include "../lib.h"

using namespace pcf;

PCF_PROGRAM(pointscan_to_ply, false) {
	void main() {
		std::string in_filename = shell::read_line("Pointscan file");
		if(in_filename.empty()) return;
		
		auto dot_pos = in_filename.find_last_of('.');
		if(dot_pos == std::string::npos) throw std::runtime_error("No filename extension.");

		std::string out_filename = in_filename.substr(0, dot_pos) + ".ply";
		std::cout << "PLY file: " << out_filename << std::endl;
		
		std::cout << "Importing from Pointscan..." << std::endl;
		pointscan_importer in(in_filename);
		unorganized_point_cloud_full pc(in);
		
		std::cout << "Exporting to PLY..." << std::endl;
		ply_exporter ply(out_filename);
		pc.export_with(ply);
		
		std::cout << "Done." << std::endl;
	}
};
