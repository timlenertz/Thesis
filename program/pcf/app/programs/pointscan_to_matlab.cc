#include "../program.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <memory>

#include "../lib.h"

using namespace pcf;

PCF_PROGRAM(pointscan_to_matlab) {
	void main() {
		std::string in_filename = shell::read_line("Pointscan file", "../../../townhall/Scan_005.scan");
		if(in_filename.empty()) return;
		
		pointscan_importer in(in_filename);
		std::size_t w = in.columns();
		std::size_t h = in.rows();

		const std::string prefix = "../matlab/";

		const float zero = 0;


		{
			std::ofstream dim_out(prefix + "size.dat");
			dim_out << w << ' ' << h;
		}

		{
			std::ofstream az_out(prefix + "az.dat");
			std::ofstream el_out(prefix + "el.dat");
			std::ofstream rad_out(prefix + "r.dat");
			std::ofstream val_out(prefix + "v.dat");		
		
			std::unique_ptr<spherical_coordinates[]> row(new spherical_coordinates[w]);
			spherical_coordinates* row_end = row.get() + w;

			for(std::ptrdiff_t y = 0; y < h; ++y) {
				std::cout << "Spherical row " << y+1 << " from " << h << "..." << std::endl;
				in.read_row_spherical(row.get());
						
				for(spherical_coordinates* s = row.get(); s != row_end; ++s) {
					if(s->valid()) {
						az_out.write(reinterpret_cast<const char*>(&s->azimuth.get_radiants()), sizeof(float));
						el_out.write(reinterpret_cast<const char*>(&s->elevation.get_radiants()), sizeof(float));
						rad_out.write(reinterpret_cast<const char*>(&s->radius), sizeof(float));
						val_out.put(1);
					} else {
						az_out.write(reinterpret_cast<const char*>(&zero), sizeof(float));
						el_out.write(reinterpret_cast<const char*>(&zero), sizeof(float));
						rad_out.write(reinterpret_cast<const char*>(&zero), sizeof(float));
						val_out.put(0);
					}
				}
			}
		}
		
		{
			std::ofstream x_out(prefix + "x.dat");
			std::ofstream y_out(prefix + "y.dat");
			std::ofstream z_out(prefix + "z.dat");
		
			std::unique_ptr<point_full[]> row(new point_full[w]);
			point_full* row_end = row.get() + w;

			for(std::ptrdiff_t y = 0; y < h; ++y) {
				std::cout << "Cartesian row " << y+1 << " from " << h << "..." << std::endl;
				in.read_row(row.get());
			
				for(point_full* p = row.get(); p != row_end; ++p) {
					if(p->valid()) {
						x_out.write(reinterpret_cast<const char*>(&(*p)[0]), sizeof(float));
						y_out.write(reinterpret_cast<const char*>(&(*p)[1]), sizeof(float));
						z_out.write(reinterpret_cast<const char*>(&(*p)[2]), sizeof(float));
					} else {
						x_out.write(reinterpret_cast<const char*>(&zero), sizeof(float));
						y_out.write(reinterpret_cast<const char*>(&zero), sizeof(float));
						z_out.write(reinterpret_cast<const char*>(&zero), sizeof(float));
					}
				}
			}
		}
		
		std::cout << "Done." << std::endl;
	}
};
