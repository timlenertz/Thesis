#include <string>

pcf::range_point_cloud_full hdv_scan(const std::string& id) {
	std::string path = "../../hdv/Scan_" + id + ".scan";
	auto pc = pcf::import_range_point_cloud(path);
	pc.set_name(id);
	return pc;
}