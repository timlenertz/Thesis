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


void draw_histogram_to_file(const histogram_t& hist, const std::string& name, const std::string& outfile, float mn = 0, float mx = 0, unsigned bins = 1000) {
	if(mx == 0) for(const auto& el : hist) if(el.value > mx) mx = el.value;

	TH1* h = new TH1F(name.c_str(), name.c_str(), bins, mn, mx);
	for(const auto& el : hist)
		h->Fill(el.value, el.weight);

	gROOT->SetBatch(kTRUE);
	TCanvas* canvas = new TCanvas("canvas");
	//h->SetStats(kFALSE);
	h->Draw();
	canvas->Print(outfile.c_str());
	gROOT->SetBatch(kFALSE);
}


template<typename Iterator>
TH1* weights_histogram(Iterator begin, Iterator end, const std::string& name = "density") {
	histogram_t hist;
	for(Iterator it = begin; it != end; ++it) hist.emplace_back(it->get_weight());
	return histogram(hist, name);
}


template<typename Iterator, typename Other_cloud>
histogram_t closest_point_distances(Iterator begin, Iterator end, const Other_cloud& pc, bool nosame = false, float dmax = INFINITY) {
	histogram_t distances;
	for(Iterator it = begin; it != end; ++it) {
		const auto& p = *it;
		if(! p.valid()) continue;
		const auto& q = (nosame
			? pc.closest_point(p, 0, dmax, [&p](const pcf::point_xyz& q) { return (p != q); } )
			: pc.closest_point(p, 0, dmax)
		);
		float d = distance(p, q);
		if(d > dmax) continue;
		distances.emplace_back(d);
	}
	return distances;
}

/////////////////////////////////////

bool lmin_formula_condition(const pcf::point_full& p, const Eigen::Vector3f& n) {
	using pcf::sq;
	float nx = n[0], ny = n[1], nz = n[2];

	const static pcf::angle alpha = pcf::angle::degrees(70);
	const static pcf::angle beta = pcf::angle::degrees(5);
	
	const static float cosa = std::cos(alpha);
	const static float tanb = std::tan(beta);
	const static float tan1 = std::tan(pcf::quarter_pi - beta);
	const static float tan2 = std::tan(pcf::quarter_pi + beta);

	if(p.get_weight() > 0.002) return false;

	if(not(std::abs(nz) > cosa)) {
		float r  = std::abs(nx / ny);
		float ir = std::abs(ny / nx);
		if(not(r < tanb && ir < tanb && tan1 < r && r < tan2)) return false;
	}
	
	return true;
}


float lmin_formula(const Eigen::Vector3f& n, float p_l) {
	using pcf::sq;
	float nx = n[0], ny = n[1], nz = n[2];

	return p_l * std::min({
		std::sqrt(1.0 + sq(nx)/sq(nz)),
		std::sqrt(1.0 + sq(ny)/sq(nz)),
		std::sqrt(2.0 + sq(nx+ny)/sq(nz)),
		std::sqrt(2.0 + sq(nx-ny)/sq(nz))
	});
}


bool normals_compatible(const Eigen::Vector3f& n1, const Eigen::Vector3f& n2) {
	const auto max_angle = pcf::angle::degrees(20);
	float dot = n1.dot(n2);
	return std::abs(dot) > std::cos(max_angle);
}


////////////////////////////////////


template<typename Cloud>
histogram_t lmin_histogram(const Cloud& pc, float p_l, const pcf::pose& camera_pose) {
	using pcf::sq;
	auto T = camera_pose.transformation_from_world().linear();

 	histogram_t values;
	
	for(const auto& q : pc) { if(! q.valid()) continue;
		Eigen::Vector3f n = T * q.get_normal();
		values.emplace_back(lmin_formula(n, p_l));
	}
	
	return values;
}


template<typename Cloud>
histogram_t angles_histogram(const Cloud& pc, const pcf::pose& camera_pose) {
	using pcf::sq;
	auto T = camera_pose.transformation_from_world().linear();

 	histogram_t values;
	
	for(const auto& q : pc) { if(! q.valid()) continue;
		Eigen::Vector3f n = T * q.get_normal();
		values.emplace_back(std::acos(n[2]));
	}
	
	return values;
}



template<typename Cloud>
histogram_t p_l_histogram(const Cloud& pc, const pcf::pose& camera_pose) {
	using pcf::sq;
	auto T = camera_pose.transformation_from_world().linear();

 	histogram_t p_l_values;
	
	for(const auto& q : pc) { if(! q.valid()) continue;
		Eigen::Vector3f n = T * q.get_normal();

		const auto& p = pc.closest_point(q, 0, INFINITY, [&q](const pcf::point_xyz& p) { return (p != q); } );
		float d = distance(p, q);

		if(! lmin_formula_condition(p, n)) continue;
		
		float p_l = d / lmin_formula(n, 1.0);
		p_l_values.emplace_back(p_l);
	}
	
	return p_l_values;
}



void invalidate_using_lmin_condition(pcf::point_cloud_full& pc, const pcf::pose& camera_pose) {
	auto T = camera_pose.transformation_from_world().linear();

	for(auto& p : pc) { if(! p.valid()) continue;
		Eigen::Vector3f n = T * p.get_normal();
		if(! lmin_formula_condition(p, n)) p.invalidate();
	}
}



template<typename Cloud>
float estimate_p_l(const Cloud& pc, const pcf::pose& camera_pose) {
	using pcf::sq;
	auto T = camera_pose.transformation_from_world().linear();

 	std::vector<float> p_l_values;
	
	for(const auto& q : pc) { if(! q.valid()) continue;
		const auto& p = pc.closest_point(q, 0, INFINITY, [&q](const pcf::point_xyz& p) { return (p != q); } );
		float d = distance(p, q);
		Eigen::Vector3f n = T * p.get_normal();
		
		if(! lmin_formula_condition(p, n)) continue;

		float p_l = d / lmin_formula(n, 1.0);
		p_l_values.emplace_back(p_l);
	}
	
	auto median_it = p_l_values.begin() + p_l_values.size()/2;
	std::nth_element(p_l_values.begin(), median_it, p_l_values.end());
	float est_p_l = *median_it;
	
	return est_p_l;
}



template<typename Cloud>
histogram_t nearest_neighbor_distances(const Cloud& pc, const pcf::pose& camera_pose, bool adj = false) {
	using pcf::sq;
	auto T = camera_pose.transformation_from_world().linear();

	histogram_t distances;
	for(const auto& q : pc) { if(! q.valid()) continue;
		const auto& p = pc.closest_point(q, 0, INFINITY, [&q](const pcf::point_xyz& p) { return (p != q); } );
		Eigen::Vector3f n = T * p.get_normal();
		float d = distance(p, q);

		if(! lmin_formula_condition(p, n)) continue;
		if(adj) d /= lmin_formula(n, 1.0);
		distances.emplace_back(d);
	}
	return distances;
}


template<typename Iterator, typename Other_cloud>
histogram_t adjusted_closest_point_distances(Iterator begin, Iterator end, const Other_cloud& pc, float p_l, const pcf::pose& camera_pose, bool rej = false) {
	using pcf::sq;
	auto T = camera_pose.transformation_from_world().linear();

	bool out=false;
	
	float maxd = 0.5;

	histogram_t distances;
	for(Iterator it = begin; it != end; ++it) {
		auto&& p = *it;
		if(! p.valid()) continue;
		auto&& q = pc.closest_point(p, 0, maxd);
		float d = distance(p, q);

		if(! normals_compatible(p.get_normal(), q.get_normal())) continue;

		//p.set_color(0x333333);
		
		Eigen::Vector3f n = T * p.get_normal();
		if(! lmin_formula_condition(p, n)) continue;
		
		if(rej) {
			Eigen::Vector3f diff = q.coordinates() - p.coordinates();
			d = (p.coordinates() - q.coordinates() + p.get_normal().dot(diff) * p.get_normal()).norm();		
		}
		
		//if(d > maxd) continue;

		//p.set_color(0xff0000);
		
		auto lmin = lmin_formula(n, p_l);
				
		distances.emplace_back(2.0 * d / lmin);
	}
	return distances;
}

/////////////////////////////////////


float adjusted_histogram_measure(const histogram_t& hist) {
	const float one_d = 0.05;
	unsigned k = 100;
	
	std::vector<unsigned> observed(k);
	unsigned total = 0;
	
	for(const auto& h : hist) {
		if(h.value > 1.0) continue;

		unsigned i = k * h.value;
		if(i < k) observed[i]++;
		total++;
	}
	
	float bin_range = 1.0 / k;
	float bin_center = bin_range / 2;
	float chi_square = 0;
	for(unsigned i = 0; i < k; ++i) {
		float o = float(observed[i]) / total;
		float e = bin_range * bin_center * 2.0;
		
		
		bin_center += bin_range;
		
		chi_square += pcf::sq(o - e) / e;
	}
	
	return chi_square;
}



template<typename In, typename Out, typename Inter = float>
Out map_num(In x, In mn_in, In mx_in, Out mn_out, Out mx_out) {
	Inter range_in = mx_in - mn_in;
	Inter range_out = mx_out - mn_out;
	Inter a = (x - mn_in) / range_in;
	return (a * range_out) + mn_out;
}







std::vector<Eigen::Vector2f> project_vectors_onto_orthonormal_coordinate_system(
	const std::vector<Eigen::Vector3f>& v,
	const Eigen::Vector3f& n)
{
	assert(v.size() >= 2);

	// vp = 3D coordinates of the vectors projected on the plane	
	std::vector<Eigen::Vector3f> vp(v.size());
	for(std::ptrdiff_t i = 0; i < v.size(); ++i) {
		vp[i] = v[i];
		vp[i][2] = -(n[0]*v[i][0] + n[1]*v[i][1])/n[2];
		std::cout << "vp[" << i << "] = " << vp[i][0] << " , " << vp[i][1] << " , " << vp[i][2] << std::endl;
	}


	// I = first axis of plane's coordinate system.
	//     use direction of vp[0]
	Eigen::Vector3f I = vp[0].normalized();
	
	// J = second axis of plane's coordinate system
	//     use cross product n x i
	Eigen::Vector3f J = n.cross(I);
	
	std::cout << "i = " << I << std::endl;
	std::cout << "j = " << J << std::endl;
	std::cout << "=====" << std::endl;
	
	// vpp = 2D coordinates of the vectors on the plane's orthonormal coordinate system
	std::vector<Eigen::Vector2f> vpp(v.size());
	for(std::ptrdiff_t i = 0; i < v.size(); ++i) {
		vpp[i][0] = vp[i].dot(I);
		vpp[i][1] = vp[i].dot(J);		
	}
	
	return vpp;
}