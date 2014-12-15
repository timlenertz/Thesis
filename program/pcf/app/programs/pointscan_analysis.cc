#include "../program.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <memory>

#include "../../pcf/pcf.h"

using namespace pcf;

PCF_PROGRAM(pointscan_analysis) {
	void main() {
		std::string in_filename = shell::read_line("Pointscan file", "../../../townhall/Scan_005.scan");
		if(in_filename.empty()) return;
		
		pointscan_importer in(in_filename);
		
		std::ofstream az_out("azimuth.dat");
		std::ofstream el_out("elevation.dat");
		std::ofstream rad_out("radius.dat");
		std::ofstream val_out("valid.dat");

		std::size_t w = in.columns();
		std::size_t h = in.rows();
		std::unique_ptr<spherical_coordinates[]> row(new spherical_coordinates[w]);
		spherical_coordinates* row_end = row.get() + w;

		for(std::ptrdiff_t y = 0; y < h; ++y) {
			std::cout << "Row " << y+1 << " from " << h << "..." << std::endl;
			in.read_row_spherical(row.get());
			
			for(spherical_coordinates* s = row.get(); s != row_end; ++s) {
				if(s->valid()) {
					az_out << s->azimuth.get_radiants() << ' ';
					el_out << s->elevation.get_radiants() << ' ';
					rad_out << s->radius << ' ';
					val_out << "1 ";
				} else {
					az_out << "0 ";
					el_out << "0 ";
					rad_out << "0 ";
					val_out << "0 ";
				}
			}
			az_out << std::endl;
			el_out << std::endl;
			rad_out << std::endl;
			val_out << std::endl;
		}
		
		std::cout << "Done." << std::endl;
	}
};
