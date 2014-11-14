#include <utility>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>
#include <utility>
#include "../../point_algorithm.h"

namespace pcf {


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


template<typename Point, typename Allocator> template<typename Other_cloud>
grid_point_cloud<Point, Allocator>::grid_point_cloud(Other_cloud&& pc, float cell_len, const Allocator& alloc) :
super(std::forward<Other_cloud>(pc), true, alloc), cell_length_(cell_len) {
	build_grid_();
}


template<typename Point, typename Allocator> template<typename Other_cloud>
grid_point_cloud<Point, Allocator>::grid_point_cloud(Other_cloud&& pc, float cell_len) :
super(std::forward<Other_cloud>(pc), true), cell_length_(cell_len) {
	build_grid_();
}


template<typename Point, typename Allocator> template<typename Other_point>
auto grid_point_cloud<Point, Allocator>::cell_for_point_(const Other_point& p) const -> cell_coordinates {
	cell_coordinates c;
	for(std::ptrdiff_t i = 0; i < 3; ++i)
		c[i] = std::floor( (p[i] - origin_[i]) / cell_length_ );
	return c;
}


template<typename Point, typename Allocator>
std::ptrdiff_t grid_point_cloud<Point, Allocator>::index_for_cell_(const cell_coordinates& c) const {
	std::ptrdiff_t idx = 0;
	idx += c[0] * number_of_cells_[1]*number_of_cells_[2];
	idx += c[1] * number_of_cells_[2];
	idx += c[2];
	return idx;
}


template<typename Point, typename Allocator>
auto grid_point_cloud<Point, Allocator>::segment_for_index_(std::ptrdiff_t i) -> segment {
	return segment(
		super::begin() + (i == 0 ? 0 : cell_offsets_[i - 1]),
		super::begin() + cell_offsets_[i]
	);
}


template<typename Point, typename Allocator>
auto grid_point_cloud<Point, Allocator>::segment_for_index_(std::ptrdiff_t i) const -> const_segment {
	return const_segment(
		super::cbegin() + (i == 0 ? 0 : cell_offsets_[i - 1]),
		super::cbegin() + cell_offsets_[i]
	);
}


template<typename Point, typename Allocator>
void grid_point_cloud<Point, Allocator>::build_grid_() {
	bounding_box bounding_cub = super::box(0.1);
	origin_ = bounding_cub.origin;
	
	Eigen::Vector3f bounding_side_lengths = bounding_cub.side_lengths();
	for(std::ptrdiff_t i = 0; i < 3; ++i) number_of_cells_[i] = std::ceil(bounding_side_lengths[i] / cell_length_);
	
	// First split into X segments
	std::vector<segment> x_segs(number_of_cells_[0]);
	super::partition_into_segments([this](const Point& p) {
		return std::floor( (p[0] - origin_[0]) / cell_length_ );
	}, number_of_cells_[0], x_segs.begin());

	// Split each into Y segments
	std::vector<segment> y_segs(number_of_cells_[0] * number_of_cells_[1]);
	std::size_t y_off = number_of_cells_[1];
	
	#pragma omp parallel for
	for(std::ptrdiff_t i = 0; i < x_segs.size(); ++i) {
		segment& x_seg = x_segs[i];

		std::vector<segment> y_segs_part(number_of_cells_[1]);
		x_seg.partition_into_segments([this](const Point& p) {
			return std::floor( (p[1] - origin_[1]) / cell_length_ );
		}, number_of_cells_[1], y_segs_part.begin());

		#pragma omp critical
		{
			auto out_y_seg = y_segs.begin() + y_off*i;
			for(const segment& y_seg : y_segs_part) *(out_y_seg++) = y_seg;
		}
	}

	// Split each into final Z segments
	cell_offsets_ = std::vector<std::ptrdiff_t>(number_of_cells_[0] * number_of_cells_[1] * number_of_cells_[2]);
	std::size_t cell_off = number_of_cells_[2];
	
	#pragma omp parallel for
	for(std::ptrdiff_t i = 0; i < y_segs.size(); ++i) {
		segment& y_seg = y_segs[i];
		
		std::vector<segment> cells_part(number_of_cells_[2]);
		y_seg.partition_into_segments([this](const Point& p) {
			return std::floor( (p[2] - origin_[2]) / cell_length_ );
		}, number_of_cells_[2], cells_part.begin());

		#pragma omp critical
		{
			auto out_cell = cell_offsets_.begin() + cell_off*i;
			for(const segment& cell : cells_part) *(out_cell++) = (cell.end() - super::cbegin());
		}
	}
}



template<typename Point, typename Allocator>
bool grid_point_cloud<Point, Allocator>::verify() const {
	for(std::size_t i = 0; i < cell_offsets_.size(); ++i) {	
		segment seg = segment_for_index_(i);	
		bool ok = std::all_of(seg.cbegin(), seg.cend(), [this, i](const Point& p) {
			return (index_for_cell_(cell_for_point_(p)) == i);
		});
		if(! ok) return false;
	}
	return true;
}



template<typename Point, typename Allocator>
bool grid_point_cloud<Point, Allocator>::in_bounds_(const cell_coordinates& c) const {
	for(std::ptrdiff_t i = 0; i < 3; ++i) if((c[i] < 0) || (c[i] >= number_of_cells_[i])) return false;
	return true;
}



template<typename Point, typename Allocator>
void grid_point_cloud<Point, Allocator>::move_into_bounds_(cell_coordinates& c) const {
	for(std::ptrdiff_t i = 0; i < 3; ++i) {
		if(c[i] < 0) c[i] = 0;
		else if(c[i] >= number_of_cells_[i]) c[i] = number_of_cells_[i] - 1;
	}
}


template<typename Point, typename Allocator>
auto grid_point_cloud<Point, Allocator>::full_subspace() const -> subspace {
	return subspace(
		*this,
		cell_coordinates(),
		cell_coordinates(number_of_cells_[0] - 1, number_of_cells_[1] - 1, number_of_cells_[2] - 1)
	);
}


template<typename Point, typename Allocator>
auto grid_point_cloud<Point, Allocator>::cell_subspace(const cell_coordinates& c) const -> subspace {
	return subspace(*this, c, c);
}


template<typename Point, typename Allocator>
auto grid_point_cloud<Point, Allocator>::segment_for_cell(const cell_coordinates& c) -> segment {
	std::ptrdiff_t i = index_for_cell_(c);
	return segment_for_index_(i);
}


template<typename Point, typename Allocator>
auto grid_point_cloud<Point, Allocator>::segment_for_cell(const cell_coordinates& c) const -> const_segment {
	std::ptrdiff_t i = index_for_cell_(c);
	return segment_for_index_(i);
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
	for(std::ptrdiff_t e : cell_offsets_) {
		if(b == e) ++n;
		b = e;
	}
	return n;
}



template<typename Point, typename Allocator> template<typename Other_point>
const Point& grid_point_cloud<Point, Allocator>::find_closest_point(const Other_point& ref) const {
	cell_coordinates c = cell_for_point_(ref);
	move_into_bounds_(c);
	
	subspace s = cell_subspace(c);
	while(s.size() == 0 && s.expand());
	
	const_segment_union u = segment_union_for_subspace(s);
	auto it = closest_point(ref, u.begin(), u.end());
	return *it;
}


template<typename Point, typename Allocator> template<typename Callback_func>
void grid_point_cloud<Point, Allocator>::iterate_cells_(Callback_func callback, bool par) const {
	std::ptrdiff_t i = 0;
	#pragma omp parallel for schedule(static, 1)
	for(std::ptrdiff_t x = 0; x < number_of_cells_[0]; ++x) {
		cell_coordinates c(x, 0, 0);		
		const_segment seg = segment_for_cell(c);
	
		for(c[1] = 0; c[1] < number_of_cells_[1]; ++c[1]) {
			for(c[2] = 0; c[2] < number_of_cells_[2]; ++c[2]) {
				callback(c, i, seg);
				seg = const_segment(seg.end(), super::begin() + cell_offsets_[i]);
				++i;
			}
		}
	}
}


template<typename Point, typename Allocator> template<typename Condition_func, typename Callback_func>
void grid_point_cloud<Point, Allocator>::find_nearest_neighbors
(std::size_t k, Condition_func cond, Callback_func callback, bool parallel) const {
	const float cbrt3 = std::cbrt(3.0f);
	
	// For each cell...
	iterate_cells_([this, k, cbrt3, &cond, &callback](const cell_coordinates& c, std::ptrdiff_t i, const segment& seg) {
		subspace s_at_least_k = cell_subspace(c); // Smallest cubic subspace around c which contains >= k points
		subspace s_ultimate = cell_subspace(c); // Smallest cubic subspace around c which contains kNN for all point in c
		std::size_t p = 0; // Number of expansions to form s_at_least_k
		
		std::vector<Point*> knn;
	
		// Now iterate through points in this cell
		for(const Point& pt : seg) {
			auto cmp = [&pt](const Point* a, const Point* b) {
				float da = euclidian_distance_sq(pt, *a);
				float db = euclidian_distance_sq(pt, *b);
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
				float pt_rel = pt[i] - origin_[i];
				float off_l = pt_rel - (cell_length_ * c[i]);
				float off_r = (cell_length_ * (c[i]+1)) - pt_rel;
				off[i] = std::max(off_l, off_r);
			}

			// Insphere radius
			float d_short =  std::min({ cell_length_ - off[0], cell_length_ - off[1], cell_length_ - off[2] });
			float r_in_sq = (cell_length_ * p) + d_short;
			r_in_sq *= r_in_sq;
						
			// Load points inside insphere
			for(Point& pt2 : s_at_least_k) {
				float d = euclidian_distance_sq(pt, pt2);
				if(d <= r_in_sq) knn.push_back(&pt2);
			}
			
			if(knn.size() == k) {
				// Exactly k points in insphere: these are kNN
				callback(pt, knn);
				continue;
			} else if(knn.size() > k) {
				// More than k points in insphere: need to sort out k nearest
				std::nth_element(knn.begin(), knn.begin()+(k-1), knn.end(), cmp);
				callback(pt, knn);
				continue;
			}
			
			// Less than k points in insphere: Need to add some from outsphere
	
			// Outsphere radius
			float r_out_sq = 0;
			for(std::ptrdiff_t i = 0; i < 3; ++i) {
				float d = off[i] + (cell_length_ * p);
				r_out_sq += d * d;
			}
			
			// Add points between insphere and outsphere
			std::size_t insphere_end = knn.size(); // End of insphere points in knn array
			for(Point& pt2 : s_ultimate) {
				float d = euclidian_distance_sq(pt, pt2);
				if(d > r_in_sq && d <= r_out_sq) knn.push_back(&pt2);
			}
			
			// Sort these additional points
			std::nth_element(knn.begin()+insphere_end, knn.begin()+(k-1), knn.end(), cmp);
			callback(pt, knn);
		}
	}, parallel);
}


}
