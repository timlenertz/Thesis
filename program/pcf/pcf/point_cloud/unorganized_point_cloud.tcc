#include <utility>
#include <cstring>
#include <algorithm>
#include "../io/point_cloud_importer.h"
#include "../util/random.h"

namespace pcf {


template<typename Point, typename Allocator>
unorganized_point_cloud<Point, Allocator>::unorganized_point_cloud(point_cloud_importer& imp, const Allocator& alloc) :
	super(imp.size(), imp.all_valid(), alloc)
{
	if(imp.has_camera_pose())
		super::set_relative_pose( imp.camera_pose() );

	std::size_t n = super::capacity();
	super::resize_(n);
	
	imp.rewind();
	imp.read(super::begin_, n);
}


template<typename Point, typename Allocator> template<typename Other_point, typename Other_allocator>
unorganized_point_cloud<Point, Allocator>::
unorganized_point_cloud(const point_cloud<Other_point, Other_allocator>& pc, bool all_val, const Allocator& alloc) :
	super(pc, all_val, alloc) { }



template<typename Point, typename Allocator>
unorganized_point_cloud<Point, Allocator>::
unorganized_point_cloud(super&& pc, bool all_val) :
	super(std::move(pc), all_val) { }



template<typename Point, typename Allocator>
void unorganized_point_cloud<Point, Allocator>::apply_transformation(const Eigen::Affine3f& t) {	
	#pragma omp parallel for
	for(auto p = super::begin(); p < super::end(); ++p)
		if(p->valid()) p->apply_transformation(t);
}


template<typename Point, typename Allocator>
void unorganized_point_cloud<Point, Allocator>::apply_pose() {	
	const Eigen::Affine3f& t = super::relative_pose().view_transformation_inverse();
	apply_transformation(t);
	super::set_relative_pose(pose());
}


template<typename Point, typename Allocator> template<typename Filter>
void unorganized_point_cloud<Point, Allocator>::filter(Filter filt, bool invalidate, bool par) {
	if(invalidate && super::all_valid_)
		throw std::invalid_argument("Cannot invalidate points in all-valid point cloud.");

	filt.reset();

	Point* np = super::begin_;

	#pragma omp parallel for if(par && invalidate)
	for(Point* p = super::begin_; p < super::end_; ++p) {
		if(! p->valid()) continue;
		bool accept = filt(*p);
		if(! accept) {
			if(invalidate) p->invalidate();
			else *(np++) = *p;
		}
	}
	
	if(! invalidate) super::end_ = np;
}



template<typename Point, typename Allocator>
void unorganized_point_cloud<Point, Allocator>::shuffle() {
	std::shuffle(super::begin(), super::end(), get_random_generator());
}



template<typename Point, typename Allocator>
void unorganized_point_cloud<Point, Allocator>::erase_invalid_points() {
	if(super::all_valid_) return;
	
	super::end_ = std::partition(super::begin(), super::end(), [](const Point& p) {
		return p.valid();
	});
}


template<typename Point, typename Allocator>
void unorganized_point_cloud<Point, Allocator>::downsample_random(float ratio, bool invalidate) {
	if(invalidate && super::all_valid_)
		throw std::invalid_argument("Cannot invalidate points in all-valid point cloud.");
	
	random_generator& rng = get_random_generator();
	Point* np = super::begin_;
	
	const std::size_t total = super::number_of_valid_points();
	const std::size_t expected = ratio * total;
	std::size_t left = total;
	std::size_t needed = expected;
	
	for(Point* p = super::begin_; p < super::end_; ++p) {
		if(! super::all_valid_) if(! p->valid()) continue;
		std::uniform_int_distribution<std::size_t> dist(0, left - 1);
		if(dist(rng) < needed) {
			if(invalidate) p->invalidate();
			else *(np++) = *p;
			--needed;
		}
		--left;
	}
	if(! invalidate) super::end_ = np;
}



template<typename Point, typename Allocator>
void unorganized_point_cloud<Point, Allocator>::downsample_grid(float cell_len, bool move, bool invalidate) {
	if(invalidate && super::all_valid_)
		throw std::invalid_argument("Cannot invalidate points in all-valid point cloud.");
	
	// Create 3D array storing closest point to center of each cell
	// or nullptr if no point in that cell
	bounding_box b = super::box();
	std::array<std::size_t, 3> sz;
	for(std::ptrdiff_t i = 0; i < 3; ++i) sz[i] = std::ceil(b.side_lengths()[i] / cell_len);
	
	array_3dim<Point*> cells(sz, nullptr);
	using cell_index = typename array_3dim<Point*>::multi_index;
	
	auto cell_center = [&b, &cell_len](const cell_index& mi) -> Eigen::Vector3f {
		Eigen::Vector3f center;
		for(auto i = 0; i < 3; ++i) center[i] = mi[i] * cell_len;
		return (center + b.origin);
	};
	
	// For each point: Retain closest point to center of each cell
	#pragma omp parallel for
	for(Point* p = super::begin_; p < super::end_; ++p) {
		cell_index mi;
		for(auto i = 0; i < 3; ++i) mi[i] = ((*p)[i] - b.origin[i]) / cell_len;
	
		auto& cand = cells[mi];
		if(cand == nullptr) {
			cand = p;
		} else {
			Eigen::Vector3f center = cell_center(mi);
		
			float old_d = distance_sq(*cand, center);
			float new_d = distance_sq(*p, center);
			
			if(new_d < old_d) cand = p;
		}
	}


	// If move: Move points retained for cells to respective cell centers
	if(move) for(auto cit = cells.begin(); cit != cells.end(); ++cit) {
		Point* p = *cit;
		if(p) *p = cell_center(cit.index());
	}
	
	
	// Invalidate all points, and then revalidate those retained for cells
	#pragma omp parallel for
	for(Point* p = super::begin_; p < super::end_; ++p) p->invalidate();

	#pragma omp parallel for
	for(auto it = cells.begin_raw(); it < cells.end_raw(); ++it)
		if(*it) (*it)->revalidate();


	// If not in invalidate mode: Need to erase invalid from pc
	if(! invalidate) erase_invalid_points();
}



template<typename Point, typename Allocator> template<typename Other_point>
const Point& unorganized_point_cloud<Point, Allocator>::
closest_point(const Other_point& query, float accepting_distance, float rejecting_distance) const {
	auto it = find_closest_point(query, super::cbegin(), super::cend(), accepting_distance);
	if(it == super::cend()) return super::invalid_point_();
	else return *it;
}


}