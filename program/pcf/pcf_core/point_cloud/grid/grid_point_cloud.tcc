#include <utility>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>
#include <utility>
#include "../../point_algorithm.h"

namespace pcf {

namespace {
	float bounding_box_expansion_ = 0.0001;
}

template<typename Cloud>
float optimal_grid_cell_length_for_knn(const Cloud& pc, std::size_t k, float alpha) {
	bounding_box box = pc.box();
	
	// Start with cell length such that there could be exactly one point per cell,
	// if they were uniformly distributed
	float len = std::cbrt(box.volume() / pc.size());
	
	std::size_t nb[3];
	for(std::ptrdiff_t i = 0; i < 3; ++i) nb[i] = std::ceil(box.side_lengths()[i] / len);
	std::size_t total_nb = nb[0] * nb[1] * nb[2];
	
	// Count how many non-empty cells with that configuration
	std::size_t nb_filled = 0;
	std::vector<bool> filled(total_nb, false);
	for(const typename Cloud::point_type& p : pc) {
		if(! p.valid()) continue;
		grid_point_cloud_xyz::cell_coordinates c;
		for(std::ptrdiff_t i = 0; i < 3; ++i) c[i] = std::floor( (p[i] - box.origin[i]) / len );
		
		std::ptrdiff_t i = (c[0] * nb[1]*nb[2]) + (c[1] * nb[2]) + c[2];
		std::vector<bool>::reference f = filled[i];
		if(! f) {
			f = true;
			++nb_filled;
			if(nb_filled == total_nb) break;
		}
	}
	
	// Deduce final estimation
	float density = float(total_nb) / (float(nb_filled) * len*len*len);
	return alpha * std::cbrt(float(k) / density);
}


template<typename Cloud>
float default_grid_cell_length(const Cloud& pc) {
	// TODO reasonable estimate
	return optimal_grid_cell_length_for_knn(pc, 10, 3.0);
}


template<typename Point, typename Allocator>
array_3dim<std::ptrdiff_t>::sizes_type grid_point_cloud<Point, Allocator>::cells_array_sizes_() const {
	array_3dim<std::ptrdiff_t>::sizes_type msz;
	auto bounding_side_lengths = super::box(bounding_box_expansion_).side_lengths();
	for(std::ptrdiff_t i = 0; i < 3; ++i) msz[i] = std::ceil(bounding_side_lengths[i] / cell_length_);
	return msz;
}



template<typename Point, typename Allocator> template<typename Other_cloud>
grid_point_cloud<Point, Allocator>::grid_point_cloud(const Other_cloud& pc, float cell_len, const Allocator& alloc) :
super(pc, true, 0, alloc),
cell_length_(cell_len != 0.0 ? cell_len : default_grid_cell_length(pc)),
box_(super::box(bounding_box_expansion_)),
cells_(cells_array_sizes_()) {
	build_grid_();
}



template<typename Point, typename Allocator>
grid_point_cloud<Point, Allocator>::grid_point_cloud(super&& pc, float cell_len) :
super(std::move(pc), true),
cell_length_(cell_len != 0.0 ? cell_len : default_grid_cell_length(pc)),
box_(super::box(bounding_box_expansion_)),
cells_(cells_array_sizes_()) {
	build_grid_();
}



template<typename Point, typename Allocator> template<typename Other_point>
auto grid_point_cloud<Point, Allocator>::cell_for_point(const Other_point& p) const -> cell_coordinates {
	cell_coordinates c;
	for(std::ptrdiff_t i = 0; i < 3; ++i)
		c[i] = std::floor( (p[i] - box_.origin[i]) / cell_length_ );
	return c;
}


template<typename Point, typename Allocator>
void grid_point_cloud<Point, Allocator>::build_grid_() {
	// First split into X segments
	std::size_t number_of_x_segments = cells_.size(0);
	std::vector<segment> x_segs(number_of_x_segments);
	super::full_segment().partition_into_segments([&](const Point& p) {
		return std::floor( (p[0] - box_.origin[0]) / cell_length_ );
	}, number_of_x_segments, x_segs.begin());
	
	// Split them into (Y, Z) segments 	
	// Handle each X segment in parallel
	//#pragma omp parallel for (doesn't work?? TODO)
	for(std::ptrdiff_t i = 0; i < x_segs.size(); ++i) {
		segment& x_seg = x_segs[i];

		std::size_t number_of_yz_segments_per_x = cells_.size(1) * cells_.size(2);
		std::vector<segment> yz_segs(number_of_yz_segments_per_x);
		
		x_seg.partition_into_segments([&](const Point& p) {
			std::ptrdiff_t y_off = std::floor( (p[1] - box_.origin[1]) / cell_length_ );
			std::ptrdiff_t z_off = std::floor( (p[2] - box_.origin[2]) / cell_length_ );
			return y_off*cells_.size(2) + z_off;
		}, number_of_yz_segments_per_x, yz_segs.begin());

		#pragma omp critical
		{
			auto out_it = cells_.begin_raw() + i*number_of_yz_segments_per_x;
			for(const segment& seg : yz_segs) *(out_it++) = (seg.end() - super::cbegin());
		}
	}
}




template<typename Point, typename Allocator>
bool grid_point_cloud<Point, Allocator>::verify() const {
	for(auto cell_it = cells_.begin(); cell_it != cells_.end(); ++cell_it) {
		cell_coordinates c = cell_it.index();
		const_segment seg = segment_for_cell(c);
		bool ok = std::all_of(seg.begin(), seg.end(), [&](const Point& p) {
			return (cell_for_point(p) == c);
		});
		if(! ok) return false;
	}
	return true;
}


template<typename Point, typename Allocator>
void grid_point_cloud<Point, Allocator>::test_colorize() {
	for(auto cell_it = cells_.begin(); cell_it != cells_.end(); ++cell_it) {
		cell_coordinates c = cell_it.index();
		segment seg = segment_for_cell(c);
		for(Point& p : seg) if((c[0]+c[1]+c[2]) % 2 == 0) p.set_color(rgb_color::red);
	}
}


template<typename Point, typename Allocator>
auto grid_point_cloud<Point, Allocator>::full_subspace() const -> subspace {
	return subspace(
		*this,
		cell_coordinates(),
		cell_coordinates(cells_.size(0) - 1, cells_.size(1) - 1, cells_.size(2) - 1)
	);
}


template<typename Point, typename Allocator>
auto grid_point_cloud<Point, Allocator>::cell_subspace(const cell_coordinates& c) const -> subspace {
	return subspace(*this, c, c);
}


template<typename Point, typename Allocator>
auto grid_point_cloud<Point, Allocator>::segment_for_cell(const cell_coordinates& c) -> segment {
	const std::ptrdiff_t& end_of_cell_offset = cells_[c];
	
	std::ptrdiff_t end_of_previous_cell_offset;
	if(c == cell_coordinates(0, 0, 0)) end_of_previous_cell_offset = 0;
	else end_of_previous_cell_offset = *(&end_of_cell_offset - 1);

	return segment(
		super::begin() + end_of_previous_cell_offset, 
		super::begin() + end_of_cell_offset
	);
}


template<typename Point, typename Allocator>
auto grid_point_cloud<Point, Allocator>::segment_for_cell(const cell_coordinates& c) const -> const_segment {
	const std::ptrdiff_t& end_of_cell_offset = cells_[c];
	std::ptrdiff_t end_of_previous_cell_offset;
	
	if(c == cell_coordinates(0, 0, 0)) end_of_previous_cell_offset = 0;
	else end_of_previous_cell_offset = *(&end_of_cell_offset - 1);

	return const_segment(
		super::cbegin() + end_of_previous_cell_offset, 
		super::cbegin() + end_of_cell_offset
	);
}


template<typename Point, typename Allocator>
auto grid_point_cloud<Point, Allocator>::segment_union_for_subspace(const subspace& s) -> segment_union {
	std::vector<segment> segs;
	for(cell_coordinates c : s) segs.push_back( segment_for_cell(c) );
	return segment_union(std::move(segs));
}


template<typename Point, typename Allocator>
auto grid_point_cloud<Point, Allocator>::segment_union_for_subspace(const subspace& s) const -> const_segment_union {
	std::vector<const_segment> segs;
	for(cell_coordinates c : s) segs.push_back( segment_for_cell(c) );
	return const_segment_union(std::move(segs));
}


template<typename Point, typename Allocator>
std::size_t grid_point_cloud<Point, Allocator>::number_of_empty_cells() const {
	std::size_t n = 0;
	std::ptrdiff_t b = 0;
	for(auto e : cells_) {
		if(b == e) ++n;
		b = e;
	}
	return n;
}



template<typename Point, typename Allocator> template<typename Other_point>
const Point& grid_point_cloud<Point, Allocator>::
closest_point(const Other_point& ref, float accepting_distance, float rejecting_distance) const {
	cell_coordinates c = cell_for_point(ref);
	c = cells_.move_into_bounds(c);
	
	subspace s = cell_subspace(c);
	while(s.number_of_points() == 0 && s.expand());
		
	const_segment_union u = segment_union_for_subspace(s);
	
	auto it = find_closest_point(ref, u.begin(), u.end());	
	return *it;
}


template<typename Point, typename Allocator> template<typename Callback_func>
void grid_point_cloud<Point, Allocator>::iterate_cells_(Callback_func callback, bool par) const {
	std::ptrdiff_t i = 0;
	#pragma omp parallel for schedule(static, 1) if(par)
	for(std::ptrdiff_t x = 0; x < cells_.size(0); ++x) {
		cell_coordinates c(x, 0, 0);		
		const_segment seg = segment_for_cell(c);
	
		for(c[1] = 0; c[1] < cells_.size(1); ++c[1]) {
			for(c[2] = 0; c[2] < cells_.size(2); ++c[2]) {
				callback(c, i, seg);
				seg = const_segment(seg.end(), super::begin() + cells_[c]);
				++i;
			}
		}
	}
}


template<typename Point, typename Allocator> template<typename Callback_func>
void grid_point_cloud<Point, Allocator>::iterate_cells_(Callback_func callback, bool par) {
	std::ptrdiff_t i = 0;
	#pragma omp parallel for schedule(static, 1) if(par)
	for(std::ptrdiff_t x = 0; x < cells_.size(0); ++x) {
		cell_coordinates c(x, 0, 0);		
		segment seg = segment_for_cell(c);
	
		for(c[1] = 0; c[1] < cells_.size(1); ++c[1]) {
			for(c[2] = 0; c[2] < cells_.size(2); ++c[2]) {
				callback(c, i, seg);
				seg = segment(seg.end(), super::begin() + cells_[c]);
				++i;
			}
		}
	}
}


template<typename Point, typename Allocator> template<typename That, typename Condition_func, typename Callback_func>
void grid_point_cloud<Point, Allocator>::nearest_neighbors_
(That that, std::size_t k, Condition_func cond, Callback_func callback, bool parallel) {
	const float cbrt3 = std::cbrt(3.0f);
	
	using segment_t = decltype(that->full_segment()); // const or non-const
	using point_ptr_t = decltype(that->data()); // const or non-const
	using selection_t = decltype(that->empty_selection());
	
	// For each cell...
	that->iterate_cells_([that, k, cbrt3, &cond, &callback](const cell_coordinates& c, std::ptrdiff_t i, segment_t& seg) {
		subspace s_at_least_k = that->cell_subspace(c); // Smallest cubic subspace around c which contains >= k points
		subspace s_ultimate = that->cell_subspace(c); // Smallest cubic subspace around c which contains kNN for all point in c
		std::size_t p = 0; // Number of expansions to form s_at_least_k
						
		typename selection_t::vector_type knn;
	
		// Now iterate through points in this cell
		for(auto&& pt : seg) {
			auto cmp = [&pt](const Point* a, const Point* b) {
				float da = distance_sq(pt, *a);
				float db = distance_sq(pt, *b);
				return (da < db);
			};
		
			// Ask if kNN of this point should be computed
			if(! cond(pt)) continue;
			
			// Clear points array
			knn.clear();
						
			// Subspaces have not yet been formed, do so now
			// --> on the whole, this is done only once per cell
			if(p == 0) {
				while(s_at_least_k.size() < k && s_at_least_k.expand()) ++p;
				s_ultimate.expand( std::floor(cbrt3*(p + 1)) + 1 );
			}
			
			// Compute dx, dy, dz
			// largest offsets from point to sides of cell
			float off[3];
			for(std::ptrdiff_t i = 0; i < 3; ++i) {
				float pt_rel = pt[i] - that->box_.origin[i];
				float off_l = pt_rel - (that->cell_length_ * c[i]);
				float off_r = (that->cell_length_ * (c[i]+1)) - pt_rel;
				off[i] = std::max(off_l, off_r);
			}

			// Insphere radius
			float d_short =  std::min({ that->cell_length_ - off[0], that->cell_length_ - off[1], that->cell_length_ - off[2] });
			float r_in_sq = (that->cell_length_ * p) + d_short;
			r_in_sq *= r_in_sq;
						
			// Load points inside insphere
			for(auto&& pt2 : that->segment_union_for_subspace(s_at_least_k)) {
				float d = distance_sq(pt, pt2);
				if(d <= r_in_sq) knn.push_back(&pt2);
			}
			
			if(knn.size() > k) {
				// More than k points in insphere: need to sort out k nearest
				std::nth_element(knn.begin(), knn.begin()+(k-1), knn.end(), cmp);

			} else if(knn.size() < k) {
				// Less than k points in insphere: Need to add some from outsphere
	
				// Outsphere radius
				float r_out_sq = 0;
				for(std::ptrdiff_t i = 0; i < 3; ++i) {
					float d = off[i] + (that->cell_length_ * p);
					r_out_sq += d * d;
				}
							
				// Add points between insphere and outsphere
				std::size_t insphere_end = knn.size(); // End of insphere points in knn array
				for(auto&& pt2 : that->segment_union_for_subspace(s_ultimate)) {
					float d = distance_sq(pt, pt2);
					if(d > r_in_sq && d <= r_out_sq) knn.push_back(&pt2);
				}
			
				// Sort these additional points
				std::nth_element(knn.begin()+insphere_end, knn.begin()+(k-1), knn.end(), cmp);
			}

			// Invoke callback.
			knn.erase(knn.begin() + k, knn.end());

			selection_t sel(knn);
			//callback(pt, sel);
		}
	}, parallel);
}




}
