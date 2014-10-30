#include <utility>
#include <cmath>
#include <algorithm>
#include <cassert>

namespace pcf {

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
auto grid_point_cloud<Point, Allocator>::segment_for_index_(std::ptrdiff_t i) const -> const segment {
	return const_cast<grid_point_cloud&>(*this).segment_for_index_(i);
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
	for(std::ptrdiff_t i = 0; i < 3; ++i) if((c[0] < 0) || (c[0] >= number_of_cells_[0])) return false;
	return true;
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
const Point& grid_point_cloud<Point, Allocator>::find_closest_point(const Other_point& from) const {
	cell_coordinates c = cell_for_point_(from);
	if(! in_bounds_(c)) return super::invalid_point_();
	
	std::ptrdiff_t i = index_for_cell_(c);
	segment seg = segment_for_index_(i);
	if(seg.empty()) return super::invalid_point_();
	
	return super::find_closest_point(from, euclidian_distance_sq, seg.begin(), seg.end());
}


template<typename Point, typename Allocator> template<typename Callback_func>
void grid_point_cloud<Point, Allocator>::iterate_cells_(Callback_func callback, bool parallel) const {
	cell_coordinates c {0, 0, 0};
	std::ptrdiff_t i = 0;
	segment seg(super::begin(), super::begin());
	
	for(std::ptrdiff_t x = 0; x < number_of_cells_[0]; ++x) {
		for(c[1] = 0; c[1] < number_of_cells_[1]; ++c[1]) {
			for(c[2] = 0; c[2] < number_of_cells_[2]; ++c[2]) {
				callback(c, i, seg);
				seg = segment(seg.end(), super::begin() + cell_offsets_[i]);
				++i;
			}
		}
		
		++c[0];
	}
}

template<typename Point, typename Allocator> template<typename Callback_func>
void grid_point_cloud<Point, Allocator>::find_nearest_neighbors(std::size_t k, Callback_func callback) const {	
	iterate_cells_([](const cell_coordinates& c, std::ptrdiff_t i, const segment& seg) {
		std::size_t p = 0;
		subspace s = c;
		
	});
}


}
