#include <string>
#include <algorithm>

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



template<typename Points_container>
TGraph* scatterplot(const Points_container& pts, const std::string& name = "scatterplot") {
	std::size_t n = pts.size();
	std::unique_ptr<float[]> x(new float [n]);
	std::unique_ptr<float[]> y(new float [n]);
	for(std::ptrdiff_t i = 0; i < n; ++i) {
		x[i] = pts[i][0];
		y[i] = pts[i][1];
	}
	TGraph* graph = new TGraph(n, x.get(), y.get());
	
	TCanvas* can = new TCanvas(name.c_str());
	graph->Draw("A*");
	
	return graph;
}


template<typename Iterator>
TH1* histogram(Iterator begin, Iterator end, const std::string& name = "histogram") {
	auto minmax = std::minmax_element(begin, end);
	std::string hname = name + "_hist";
	TH1* h = new TH1F(hname.c_str(), name.c_str(), 1000, *minmax.first, *minmax.second);
	for(Iterator it = begin; it != end; ++it) h->Fill(*it);
	
	TCanvas* can = new TCanvas(name.c_str());
	h->Draw();
	
	return h;
}


template<typename Iterator>
TH1* weights_histogram(Iterator begin, Iterator end, const std::string& name = "density") {
	std::vector<float> weights;
	for(Iterator it = begin; it != end; ++it) weights.push_back(it->get_weight());
	return histogram(weights.begin(), weights.end(), name);
}


template<typename Iterator, typename Other_cloud>
std::vector<float> closest_point_distances(Iterator begin, Iterator end, const Other_cloud& pc, bool nosame = false, float maxdist = INFINITY) {
	std::vector<float> distances;
	for(Iterator it = begin; it != end; ++it) {
		const auto& p = *it;
		if(! p.valid()) continue;
		const auto& q = (nosame ? pc.closest_point(p, 0, INFINITY, [&p](const pcf::point_xyz& q) { return (p != q); } ) : pc.closest_point(p));
		float d = distance(p, q);
		if(d < maxdist) distances.push_back(d);
	}
	return distances;
}


template<typename Iterator, typename Other_cloud>
std::vector<float> k_closest_points_distances(Iterator begin, Iterator end, const Other_cloud& pc, bool nosame = false, float maxdist = INFINITY) {
	std::vector<float> distances;
	for(Iterator it = begin; it != end; ++it) {
		const auto& p = *it;
		if(! p.valid()) continue;
		const auto& q = (nosame ? pc.closest_point(p, 0, INFINITY, [&p](const pcf::point_xyz& q) { return (p != q); } ) : pc.closest_point(p));
		float d = distance(p, q);
		if(d < maxdist) distances.push_back(d);
	}
	return distances;
}


template<typename Iterator, typename Other_cloud>
std::vector<float> adjusted_closest_point_distances(Iterator begin, Iterator end, const Other_cloud& pc, const pcf::camera& cam, float maxdist = INFINITY) {
	using pcf::sq;
	auto T = cam.view_transformation();

	bool out=false;

	std::vector<float> distances;
	for(Iterator it = begin; it != end; ++it) {
		const auto& p = *it;
		if(! p.valid()) continue;
		const auto& q = pc.closest_point(p);
		float d = distance(p, q);
		
		Eigen::Vector3f pn = p.get_normal().normalized();
		pn = (T * Eigen::Vector4f(pn[0], pn[1], pn[2], 0)).head(3);
		
		auto factor = std::sqrt(1.0 + std::min({
			sq(pn[0]),
			sq(pn[1]),
			sq(pn[0] + pn[1]),
			sq(pn[0] - pn[1])
		})/sq(pn[2]));
	
if(not out) {
std::cout<<median_closest_neighbor_distance(pcf::kdtree_point_cloud_full(pc))<<";"<<std::endl;
std::cout<<"-> "<<std::sqrt(1.0 + sq( pn[0] )/sq(pn[2]))<<std::endl;	
std::cout<<"-> "<<std::sqrt(1.0 + sq( pn[1] )/sq(pn[2]))<<std::endl;	
std::cout<<"-> "<<std::sqrt(2.0 + sq( pn[0]+pn[0] )/sq(pn[2]))<<std::endl;	
std::cout<<"-> "<<std::sqrt(2.0 + sq( pn[0]-pn[1] )/sq(pn[2]))<<std::endl;	
std::cout<<"----------"<<std::endl;
out=true;
}
		d /= factor;
		if(d < maxdist) distances.push_back(d);
	}
	return distances;
}




template<typename In, typename Out, typename Inter = float>
Out map_num(In x, In mn_in, In mx_in, Out mn_out, Out mx_out) {
	Inter range_in = mx_in - mn_in;
	Inter range_out = mx_out - mn_out;
	Inter a = (x - mn_in) / range_in;
	return (a * range_out) + mn_out;
}
