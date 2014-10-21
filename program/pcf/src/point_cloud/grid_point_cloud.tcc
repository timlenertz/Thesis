#include <utility>
#include <cmath>

namespace pcf {

template<typename Point, typename Allocator> template<typename Other_cloud>
grid_point_cloud<Point, Allocator>::grid_point_cloud(Other_cloud&& pc, const Eigen::Vector3f cell_sz, const Allocator& alloc) :
super(std::forward<Other_cloud>(pc), true, alloc), cell_size_(cell_sz) {
	build_grid_();
}


template<typename Point, typename Allocator> template<typename Other_cloud>
grid_point_cloud<Point, Allocator>::grid_point_cloud(Other_cloud&& pc, const Eigen::Vector3f cell_sz) :
super(std::forward<Other_cloud>(pc), true), cell_size_(cell_sz) {
	build_grid_();
}


template<typename Point, typename Allocator> template<typename Other_point>
auto grid_point_cloud<Point, Allocator>::cell_for_point_(const Other_point& p) const -> cell_coordinates {
	cell_coordinates c;
	for(std::ptrdiff_t i = 0; i < 3; ++i)
		c[i] = std::floor( (p[i] - origin_[i]) / cell_size_[i] );
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
void grid_point_cloud<Point, Allocator>::build_grid_() {
	cuboid bounding_cub = super::bounding_cuboid(0.1);
	origin_ = bounding_cub.origin;
	
	Eigen::Vector3f bounding_side_lengths = bounding_cub.side_lengths();
	for(std::ptrdiff_t i = 0; i < 3; ++i) number_of_cells_[i] = std::ceil(bounding_side_lengths[i] / cell_size_[i]);
	
	// First split into X segments
	std::vector<segment> x_segs(number_of_cells_[0]);
	super::partition_into_segments([this](const Point& p) {
		return std::floor( (p[0] - origin_[0]) / cell_size_[0] );
	}, number_of_cells_[0], x_segs.begin());

	
	// Split each into Y segments
	std::vector<segment> y_segs(number_of_cells_[0] * number_of_cells_[1]);
	std::size_t y_off = number_of_cells_[1];
	
	#pragma omp parallel for
	for(std::ptrdiff_t i = 0; i < x_segs.size(); ++i) {
		segment& x_seg = x_segs[i];

		std::vector<segment> y_segs_part(number_of_cells_[1]);
		x_seg.partition_into_segments([this](const Point& p) {
			return std::floor( (p[1] - origin_[1]) / cell_size_[1] );
		}, number_of_cells_[1], y_segs_part.begin());

		#pragma omp critical
		{
			auto out_y_seg = y_segs.begin() + y_off*i;
			for(const segment& y_seg : y_segs_part) *(out_y_seg++) = y_seg;
		}
	}
	
	// Split each into final X segments
	cells_ = std::vector<segment>(number_of_cells_[0] * number_of_cells_[1] * number_of_cells_[2]);
	std::size_t cell_off = number_of_cells_[2];
	
	#pragma omp parallel for
	for(std::ptrdiff_t i = 0; i < y_segs.size(); ++i) {
		segment& y_seg = y_segs[i];
		
		std::vector<segment> cells_part(number_of_cells_[1]);
		y_seg.partition_into_segments([this](const Point& p) {
			return std::floor( (p[2] - origin_[2]) / cell_size_[2] );
		}, number_of_cells_[2], cells_part.begin());

		#pragma omp critical
		{
			auto out_cell = cells_.begin() + cell_off*i;
			for(const segment& cell : cells_part) *(out_cell++) = cell;
		}
	}
}


template<typename Point, typename Allocator>
void grid_point_cloud<Point, Allocator>::verify() const {
	for(std::ptrdiff_t i = 0; i < cells_.size(); ++i) {
		bool ok = super::verify_segment_([this, i](const Point& p) -> bool {
			return (index_for_cell_(cell_for_point_(p)) == i);
		}, cells_[i]);
		if(! ok) throw std::runtime_error("Grid point cloud contains point in wrong cell.");
	}
}


template<typename Point, typename Allocator>
bool grid_point_cloud<Point, Allocator>::in_bounds_(const cell_coordinates& c) const {
	for(std::ptrdiff_t i = 0; i < 3; ++i) if((c[0] < 0) || (c[0] >= number_of_cells_[0])) return false;
	return true;
}



template<typename Point, typename Allocator> template<typename Other_point>
const Point& grid_point_cloud<Point, Allocator>::find_closest_point(const Other_point& from) const {
	cell_coordinates c = cell_for_point_(from);
	if(! in_bounds_(c)) return super::invalid_point_();
	
	std::ptrdiff_t i = index_for_cell_(c);
	const segment& seg = cells_.at(i);
	if(seg.empty()) return super::invalid_point_();
	
	return super::find_closest_point(from, euclidian_distance_sq, seg.begin(), seg.end());
}



}
