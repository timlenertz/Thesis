#include <string>

pcf::range_point_cloud_full hdv_scan(const std::string& id) {
	std::string path = "../hdv/Scan_" + id + ".scan";
	auto pc = pcf::import_range_point_cloud(path);
	pc.set_name(id);
	return pc;
}

pcf::range_point_cloud_full hdv_scan_cut(const std::string& id, const std::string& bb, const std::string& ps = "0,0,0,1,0,0,0") {
	using namespace pcf;
	auto pc = hdv_scan(id);
	pc.filter( crop_point_filter<bounding_box>(bounding_box::from_string(bb)) );
	pc.set_relative_pose( pose::from_string(ps) );
	return pc;
}

pcf::range_point_cloud_full hdv_lo() {
	return hdv_scan_cut(
		"005",
		"-17.6605,-5.41663,1.72615,-14.3355,-2.02658,6.63569",
		"16.764,3.82095,-4.20347,1,0,0,0"
	);
}


pcf::range_point_cloud_full hdv_hi() {
	return hdv_scan_cut(
		"012",
		"-7.11634,-5.53676,-2.2105,-5.63491,-2.51254,1.64889",
		"5.78633,4.91419,1.24274,0.995499,-0.00072259,-0.0845465,0.0428211"
	);
}


pcf::unorganized_point_cloud_full bunny() {
	return pcf::import_point_cloud("../misc/ply/bunny.ply");
}


pcf::unorganized_point_cloud_full dragon() {
	return pcf::import_point_cloud("../misc/ply/dragon.ply");
}



TGraph* scatterplot(const char* name, const pcf::exper::results::data_point_set& pts) {
	std::size_t n = pts.size();
	std::unique_ptr<float[]> x(new float [n]);
	std::unique_ptr<float[]> y(new float [n]);
	for(std::ptrdiff_t i = 0; i < n; ++i) {
		x[i] = pts[i].x;
		y[i] = pts[i].y;
	}
	TGraph* graph = new TGraph(n, x.get(), y.get());
	
	TCanvas* can = new TCanvas(name);
	graph->Draw("A*");
	
	return graph;
}