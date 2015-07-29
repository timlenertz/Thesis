#include <string>
#include <algorithm>

struct histogram_value_t {
	float value;
	float weight;
	
	histogram_value_t(float v, float w = 1.0) :
		value(v), weight(w) { }
};
using histogram_t = std::vector<histogram_value_t>;

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


TH1* histogram(const histogram_t& hist, const std::string& name = "histogram", float mn = 0, float mx = 0, unsigned bins = 1000) {
	if(mx == 0)
		for(const auto& el : hist) if(el.value > mx) mx = el.value;

	std::string hname = name + "_hist";
	TH1* h = new TH1F(hname.c_str(), name.c_str(), bins, mn, mx);

	for(const auto& el : hist)
		h->Fill(el.value, el.weight);
	
	TCanvas* can = new TCanvas(name.c_str());
	h->Draw();
	
	return h;
}


template<typename Iterator>
TH1* weights_histogram(Iterator begin, Iterator end, const std::string& name = "density") {
	histogram_t hist;
	for(Iterator it = begin; it != end; ++it) hist.emplace_back(it->get_weight());
	return histogram(hist, name);
}


template<typename Iterator, typename Other_cloud>
histogram_t closest_point_distances(Iterator begin, Iterator end, const Other_cloud& pc, bool nosame = false) {
	histogram_t distances;
	for(Iterator it = begin; it != end; ++it) {
		const auto& p = *it;
		if(! p.valid()) continue;
		const auto& q = (nosame
			? pc.closest_point(p, 0, INFINITY, [&p](const pcf::point_xyz& q) { return (p != q); } )
			: pc.closest_point(p)
		);
		float d = distance(p, q);
		distances.emplace_back(d);
	}
	return distances;
}


template<typename Cloud>
histogram_t estimate_p_l(const Cloud& pc, const pcf::pose& camera_pose) {
	using pcf::sq;
	auto T = camera_pose.transformation_from_world();

	//std::vector<float> p_l_values;
	histogram_t p_l_values;
	
	for(const auto& q : pc) {
		Eigen::Vector3f n = q.get_normal();
		n = (T * Eigen::Vector4f(n[0], n[1], n[2], 0)).head(3);
		float nx = n[0], ny = n[1], nz = n[2];

		const auto& p = pc.closest_point(q, 0, INFINITY, [&q](const pcf::point_xyz& p) { return (p != q); } );
		float d = distance(p, q);

		float m = std::min({
			std::sqrt(1.0 + sq(nx)/sq(nz)),
			std::sqrt(1.0 + sq(ny)/sq(nz)),
			std::sqrt(2.0 + sq(nx+ny)/sq(nz)),
			std::sqrt(2.0 + sq(nx-ny)/sq(nz))
		});
		
		float p_l = d / m;
		//p_l_values.push_back(p_l);
		p_l_values.emplace_back(p_l);
	}
	
	return p_l_values;
	/*
	auto median_it = p_l_values.begin() + p_l_values.size()/2;
	std::nth_element(p_l_values.begin(), p_l_values.end(), median_it);
	float est_p_l = *median_it;
	
	return est_p_l;*/
}


template<typename Cloud>
histogram_t nearest_neighbor_distances(const Cloud& pc, const pcf::pose& camera_pose, bool adj = false) {
	using pcf::sq;
	auto T = camera_pose.transformation_from_world();
	
	histogram_t distances;
	for(const auto& q : pc) {
		Eigen::Vector3f n = q.get_normal();
		n = (T * Eigen::Vector4f(n[0], n[1], n[2], 0)).head(3);
		float nx = n[0], ny = n[1], nz = n[2];

		float d;

		if(! adj) {
			const auto& p = pc.closest_point(q, 0, INFINITY, [&q](const pcf::point_xyz& p) { return (p != q); } );
			d = distance(p, q);
		} else {
			float lmin = std::min({
				std::sqrt(1.0 + sq(nx)/sq(nz)),
				std::sqrt(1.0 + sq(ny)/sq(nz)),
				std::sqrt(2.0 + sq(nx+ny)/sq(nz)),
				std::sqrt(2.0 + sq(nx-ny)/sq(nz))
			});
			d = 0.01 * lmin;
		}
		distances.emplace_back(d);
	}
	return distances;
}



template<typename Iterator, typename Other_cloud>
histogram_t adjusted_closest_point_distances(Iterator begin, Iterator end, const Other_cloud& pc, const pcf::pose& camera_pose) {
	using pcf::sq;
	auto T = camera_pose.transformation_from_world();

	bool out=false;

	histogram_t distances;
	for(Iterator it = begin; it != end; ++it) {
		const auto& p = *it;
		if(! p.valid()) continue;
		const auto& q = pc.closest_point(p);
		float d = distance(p, q);

		Eigen::Vector3f pn = p.get_normal();
		pn = (T * Eigen::Vector4f(pn[0], pn[1], pn[2], 0)).head(3);
		float nx = pn[0], ny = pn[1], nz = pn[2];
		
		float maxd;
		{
			float c = (1.0 - sq(ny)) * (sq(ny) + sq(nz));
			float den = 2.0 * sq(nz);
			float f1 = std::abs(1.0 + 2.0*nx*ny + sq(nz));
			float f2 = std::abs(1.0 - 2.0*nx*ny + sq(nz));
			maxd = std::sqrt(std::min(f1, f2) * c) / den;
		}
		maxd = 0.1 * maxd;
		//std::cout << d << " max:" << maxd << std::endl;
		//if(d > maxd) continue;
		
		auto lmin = std::min({
			std::sqrt(1.0 + sq(nx)/sq(nz)),
			std::sqrt(1.0 + sq(ny)/sq(nz)),
			std::sqrt(2.0 + sq(nx+ny)/sq(nz)),
			std::sqrt(2.0 + sq(nx-ny)/sq(nz))
		});
		
		auto weight = std::abs(pn[2]);
	
if(not out) {
std::cout<<median_closest_neighbor_distance(pcf::kdtree_point_cloud_full(pc))<<";"<<std::endl;
std::cout<<"-> "<<std::sqrt(1.0 + sq( pn[0] )/sq(pn[2]))<<std::endl;	
std::cout<<"-> "<<std::sqrt(1.0 + sq( pn[1] )/sq(pn[2]))<<std::endl;	
std::cout<<"-> "<<std::sqrt(2.0 + sq( pn[0]+pn[0] )/sq(pn[2]))<<std::endl;	
std::cout<<"-> "<<std::sqrt(2.0 + sq( pn[0]-pn[1] )/sq(pn[2]))<<std::endl;	
std::cout<<"----------"<<std::endl;
out=true;
}
		distances.emplace_back(d / lmin);
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
