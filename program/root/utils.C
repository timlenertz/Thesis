#include <string>

pcf::range_point_cloud_full hdv_scan(const std::string& id) {
	std::string path = "../hdv/Scan_" + id + ".scan";
	auto pc = pcf::import_range_point_cloud(path);
	pc.set_name(id);
	return pc;
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