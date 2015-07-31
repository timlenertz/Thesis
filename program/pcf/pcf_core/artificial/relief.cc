#include "relief.h"
#include "../util/random.h"
#include "../util/coordinates.h"
#include "../util/multi_dimensional_array.h"
#include "../geometry/math_constants.h"
#include "../point_cloud/merge.h"
#include <Eigen/Eigen>
#include <cmath>
#include <memory>
#include <utility>
#include <random>
#include <algorithm>
#include <fstream>


namespace pcf {

relief::relief(float width, int seed, float height) :
width_(width), height_multiplier_(height) {
	std::mt19937 gen;
	if(seed) gen.seed(seed);
	else gen = get_random_generator();
			
	std::size_t number_of_engravings = 100;
	for(auto i = 0; i < number_of_engravings; ++i) {
		engraving e;
		e.x = random_real<float>(0.0, width, gen);
		e.y = random_real<float>(0.0, width, gen);	
		if(i < number_of_engravings / 4) {	
			e.radius = random_real<float>(0.2*width, 0.3*width, gen);
			e.depth = e.radius * random_real<float>(0.2, 0.4, gen);
		} else {
			e.radius = random_real<float>(0.02*width, 0.09*width, gen);
			e.depth = e.radius * random_real<float>(0.7, 2.2, gen);
		}
		e.out = (random_integer(0, 10, gen) < 3);
		engravings_.push_back(e);
	}
	
	uncrop();
}


bool relief::in_bounds(float x, float y) const {
	auto line_side = [x, y](const Eigen::Vector2f& a, const Eigen::Vector2f& b) {
		auto ba = a - b;
		auto bp = Eigen::Vector2f(x, y) - b;
		return (ba[0]*bp[1] < ba[1]*bp[0]);
	};

	return
		line_side(corners_[0], corners_[1]) &&
		line_side(corners_[1], corners_[2]) &&
		line_side(corners_[2], corners_[3]) &&
		line_side(corners_[3], corners_[0]);
}

point_full relief::at(float x, float y, bool check_bounds) const {
	if(check_bounds && !in_bounds(x, y)) return point_full();

	float z = 0.0;
	float der_x = 0, der_y = 0;
	
	for(const engraving& e : engravings_) {
		float dx = e.x - x;
		float dy = e.y - y;
		float d_sq = dx*dx + dy*dy;
		float r_sq = e.radius * e.radius;
		if(r_sq <= d_sq) continue;
		
		float sqrt = std::sqrt(1.0 - d_sq/r_sq);
		float h = (e.out ? -e.depth : e.depth);
				
		z += h * sqrt;
		der_x += h * (e.x - x) / (r_sq * sqrt);
		der_y += h * (e.y - y) / (r_sq * sqrt);
	}
	
	z *= height_multiplier_;
	der_x *= height_multiplier_;
	der_y *= height_multiplier_;
			
	point_full p(x - width_/2.0, y - width_/2.0, z);	
	p.set_normal(-Eigen::Vector3f(der_x, der_y, -1).normalized());
	return p;
}


range_point_cloud_full relief::make_point_cloud(float density) const {
	std::size_t n_side = width_ * std::sqrt(density);
	
	range_point_cloud_full pc(n_side, n_side);
	
	for(std::ptrdiff_t yi = 0; yi < n_side; ++yi)
	for(std::ptrdiff_t xi = 0; xi < n_side; ++xi) {
		float x = (width_ * xi) / n_side;
		float y = (width_ * yi) / n_side;
		pc.at(xi, yi) = at(x, y);
	}
	
	return pc;
}


std::vector<point_full> relief::make_samples(std::size_t n) const {
	std::vector<point_full> pts;
	for(std::size_t i = 0; i != n; ++i) {
		float x = random_real<float>(0, width_);
		float y = random_real<float>(0, width_);
		if(in_bounds(x, y)) pts.push_back(at(x, y, false));
	}
	return pts;
}



camera_range_point_cloud_full<projection_image_camera> relief::make_projected_point_cloud(const projection_image_camera& cam, float mesh_density, bool correct_coordinates, unsigned mesh_scale) const {
	using plane_t = Eigen::Hyperplane<float, 3>;

	projection_image_camera mesh_cam = cam;
	mesh_cam.set_image_width(mesh_scale * cam.image_width());

	size_2dim mesh_size = mesh_cam.image_size();
	array_2dim<std::ptrdiff_t> mesh_projection(mesh_size, -1);
	std::vector<plane_t> triangle_planes;
	
	auto dist = [&](std::ptrdiff_t i, const index_2dim& p) -> float {
		Eigen::ParametrizedLine<float, 3> ray = mesh_cam.ray(p);
		return ray.intersection(triangle_planes[i]);
	};
				
	auto fill_triangle = [&](Eigen::Vector3f ra, Eigen::Vector3f rb, Eigen::Vector3f rc) {
		std::ptrdiff_t w = mesh_cam.image_width() - 1, h = mesh_cam.image_height() - 1;

		// Get 2D image coordinates a,b,c
		index_2dim a = mesh_cam.to_image(ra), b = mesh_cam.to_image(rb), c = mesh_cam.to_image(rc);
		if( (a.x == b.x && b.x == c.x) || (a.y == b.y && b.y == c.y) ) return;
		if(a.x < 0 && a.y < 0 && b.x < 0 && b.y < 0 && c.x < 0 && c.y < 0) return;
		if(a.x > w && a.y > h && b.x > w && b.y > h && c.x > w && c.y > h) return;

		// Sort ra,rb,rc and a,b,c by increasing image Y
		if(a.y > b.y) { std::swap(a, b); std::swap(ra, rb); }
		if(b.y > c.y) { std::swap(b, c); std::swap(rb, rc); }
		if(a.y > b.y) { std::swap(a, b); std::swap(ra, rb); }
		
		// Slopes in image coordinates, and if edges are horizontal
		float ab_slope, bc_slope;
		float ac_slope = float(a.x - c.x)/(a.y - c.y);
		bool ab_horiz = (a.y == b.y);
		bool bc_horiz = (b.y == c.y);
		if(!ab_horiz) ab_slope = float(a.x - b.x)/(a.y - b.y);
		if(!bc_horiz) bc_slope = float(b.x - c.x)/(b.y - c.y);
		
		// Create triangle entry
		std::ptrdiff_t i = triangle_planes.size();
		triangle_planes.push_back(plane_t::Through(ra, rb, rc));
		
		// Function to draw horizontal line segment
		auto draw_line = [&](std::ptrdiff_t y, std::ptrdiff_t x1, std::ptrdiff_t x2) {
			if(x1 > x2) std::swap(x1, x2);
			if(x1 > w || x2 < 0) return;
			if(x1 < 0) x1 = 0;
			if(x2 > w) x2 = w;
			
			for(index_2dim p(x1, y); p.x <= x2; ++p.x) {
				float old_i = mesh_projection[p];
				float old_d, d = dist(i, p);
				
				if(old_i == -1) old_d = INFINITY;
				else old_d = dist(old_i, p);
				
				if(d < old_d) mesh_projection[p] = i;
			}
		};
		
		// Scanlines from A to B
		if(ab_horiz) draw_line(b.y, a.x, b.x);
		else for(std::ptrdiff_t y = std::max(a.y, std::ptrdiff_t(0)); y < std::min(b.y, h); ++y) {
			std::ptrdiff_t ac_x = a.x + ac_slope*(y - a.y);
			std::ptrdiff_t ab_x = a.x + ab_slope*(y - a.y);
			draw_line(y, ab_x, ac_x);
		}
		
		// Scanlines from B to C
		if(bc_horiz) draw_line(b.y, b.x, c.x);
		else for(std::ptrdiff_t y = std::max(b.y, std::ptrdiff_t(0)); y <= std::min(c.y, h); ++y) {
			std::ptrdiff_t ac_x = a.x + ac_slope*(y - a.y);
			std::ptrdiff_t bc_x = b.x + bc_slope*(y - b.y);
			draw_line(y, bc_x, ac_x);
		}
	};		
		
	auto project_triangle = [&](const Eigen::Vector2f& ra, const Eigen::Vector2f& rb, const Eigen::Vector2f& rc) {
		point_full a = at(ra[0], ra[1]), b = at(rb[0], rb[1]), c = at(rc[0], rc[1]);	
		if(a.valid() && b.valid() && c.valid()) fill_triangle(a, b, c);
	};

	// Generate mesh, and render on projection image
	vertices_t vertices;
	faces_t faces;
	std::tie(vertices, faces) = generate_mesh_(mesh_density);
	for(const face_t& f : faces)
		project_triangle(vertices[f[0]], vertices[f[1]], vertices[f[2]]);


	//masked_intensity_image(mesh_projection_z, 0.0f).export_visualization_to_image_file("m.png");
		
	camera_range_point_cloud_full<projection_image_camera> pc(cam);
	index_2dim pc_ic, ic;
	for(ic.y = 0, pc_ic.y = mesh_scale/2; ic.y < cam.image_height() - 1; ++ic.y, pc_ic.y += mesh_scale)
	for(ic.x = 0, pc_ic.x = mesh_scale/2; ic.x < cam.image_width() - 1 ; ++ic.x, pc_ic.x += mesh_scale) {
		point_full& p = pc.at(ic.x, ic.y);
		
		std::ptrdiff_t i = mesh_projection[pc_ic];

		if(i == -1) {
			p.invalidate();
		} else {
			Eigen::ParametrizedLine<float, 3> ray = mesh_cam.ray(pc_ic);
			Eigen::Vector3f rp = ray.intersectionPoint(triangle_planes[i]);
			rp = cam.view_transformation() * rp;

			p = point_full(rp);

			Eigen::Vector3f wp = pc.absolute_pose().transformation_to_world() * p.coordinates();
			point_full relief_p = at(wp[0] + width_/2, wp[1] + width_/2);
			if(relief_p.valid()) {
				wp[2] = relief_p[2];
				if(correct_coordinates) p = pc.absolute_pose().transformation_from_world() * wp;
				
				Eigen::Vector4f n(relief_p.get_normal()[0], relief_p.get_normal()[1], relief_p.get_normal()[2], 0.0);
				
				p.set_normal( (pc.absolute_pose().transformation_from_world() * n).head(3) );
			} else {
				p.invalidate();			
			}
		}
	}

	return pc;
}

relief::mesh_t relief::generate_mesh_(float density) const {
	std::ptrdiff_t l = width_ * std::sqrt(density); // number of edges at side
	if(l % 2 != 0) ++l;
	
	// *-----*-----*
	// *\    *    /*
	// *  \  *  /  *
	// *    \*/    *
	// *-----*-----*
	// *    /*\    *
	// *  /  *  \  *
	// */    *    \*
	// *-----*-----*
	// l = 2
	// 9 vertices
	// 8 faces

	vertices_t vertices;
	faces_t faces;
	
	for(std::ptrdiff_t yi = 0; yi <= l; ++yi)
	for(std::ptrdiff_t xi = 0; xi <= l; ++xi)
		vertices.emplace_back(
			(width_ * xi) / l,
			(width_ * yi) / l
		);
	
	std::ptrdiff_t i = l + 2;
	for(std::ptrdiff_t yi = 1; yi <= l; ++yi, ++i)
	for(std::ptrdiff_t xi = 1; xi <= l; ++xi, ++i)
		if(i % 2 == 0) {
			faces.push_back({i-l-2, i, i-1});
			faces.push_back({i, i-l-2, i-l-1});
		} else {
			faces.push_back({i-l-1, i, i-1});
			faces.push_back({i-1, i-l-2, i-l-1});
		}

	return std::make_pair(vertices, faces);
}

void relief::export_to_ply(const std::string& path, float density) const {
	std::ofstream fstr(path);

	vertices_t vertices;
	faces_t faces;
	std::tie(vertices, faces) = generate_mesh_(density);

	fstr <<
		"ply\n"
		"format ascii 1.0\n"
		"comment PLY file generated using pcf::relief\n"
		"element vertex " << vertices.size() << "\n"
		"property float x\n"
		"property float y\n"
		"property float z\n"
		"property float nx\n"
		"property float ny\n"
		"property float nz\n"
		"property uchar in_bounds\n"
		"element face " << faces.size() << "\n"
		"property list uchar int vertex_index\n"
		"end_header" << std::endl;

	for(const Eigen::Vector2f& v : vertices) {
		point_full p = at(v[0], v[1], false);
		Eigen::Vector3f n = p.get_normal();
		fstr << p[0] << ' ' << p[1] << ' ' << p[2] << ' '
			<< n[0] << ' ' << n[1] << ' ' << n[2] << ' '
			<< in_bounds(v[0], v[1]) << std::endl;
	}
	
	for(const face_t& f : faces) {
		fstr << "3 " << f[0] << ' ' << f[1] << ' ' << f[2] << std::endl;
	}
}


void relief::crop(float t, int seed) {
	corners_[0] = Eigen::Vector2f(t, t);
	corners_[1] = Eigen::Vector2f(width_ - t, t);
	corners_[2] = Eigen::Vector2f(width_ - t, width_ - t);
	corners_[3] = Eigen::Vector2f(t, width_ - t);

	float d = t * 0.9;
	std::mt19937 gen;
	if(seed) gen.seed(seed);
	else gen = get_random_generator();
	for(Eigen::Vector2f& c : corners_) {
		c[0] += random_real<float>(-d, +d, gen);
		c[1] += random_real<float>(-d, +d, gen);
	}
}


void relief::uncrop() {
	corners_[0] = Eigen::Vector2f(0, 0);
	corners_[1] = Eigen::Vector2f(width_, 0);
	corners_[2] = Eigen::Vector2f(width_, width_);
	corners_[3] = Eigen::Vector2f(0, width_);
}


}