#include <utility>
#include <cstring>
#include <algorithm>
#include <stdexcept>
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
unorganized_point_cloud(const point_cloud<Other_point, Other_allocator>& pc, std::size_t cap, bool all_val, const Allocator& alloc) :
	super(pc, cap, all_val, alloc) { }



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
		if(invalidate && !accept) p->invalidate();
		else if(!invalidate && accept) *(np++) = *p;
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
void unorganized_point_cloud<Point, Allocator>::downsample_grid(float cell_len, bool keep_first, bool invalidate) {
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
		} else if(! keep_first) {
			Eigen::Vector3f center = cell_center(mi);
		
			float old_d = distance_sq(*cand, center);
			float new_d = distance_sq(*p, center);
			
			if(new_d < old_d) cand = p;
		}
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


template<typename Point, typename Allocator> template<typename Distribution>
void unorganized_point_cloud<Point, Allocator>::randomly_displace_points(const Distribution& dist_orig) {
	Distribution dist(dist_orig);
	random_generator& rng = get_random_generator();

	for(Point* p = super::begin_; p < super::end_; ++p) {
		if(! p->valid()) continue;
		
		Eigen::Vector3f d(
			dist(rng), dist(rng), dist(rng)
		);
		p->homogeneous_coordinates.head(3) += d;
	}
}


template<typename Point, typename Allocator> template<typename Distribution>
void unorganized_point_cloud<Point, Allocator>::add_random_noise_around_points(std::size_t amount, const Distribution& displacement) {
	if(amount > super::remaining_capacity())
		throw std::logic_error("Not enough capacity for requested amound of additional points.");
	if(amount > super::size())
		throw std::logic_error("Cannot add more noise points than existing points.");
	
	Distribution dist(displacement);
	random_generator& rng = get_random_generator();
	
	Point* np = super::end_;
	iterate_and_pick_random(super::begin_, super::end_, amount, [&](Point& p) {
		if(! p.valid()) return;
		
		*np = p;	
		Eigen::Vector3f d( dist(rng), dist(rng), dist(rng) );
		np->homogeneous_coordinates.head(3) += d;
		
		++np;
	});
	
	super::end_ = np;
}


template<typename Point, typename Allocator> template<typename Camera>
void unorganized_point_cloud<Point, Allocator>::erase_invisible_points(const Camera& cam, bool invalidate) {
	if(invalidate && super::all_valid_)
		throw std::invalid_argument("Cannot invalidate points in all-valid point cloud.");

	// Project points using camera and z-buffer
	// Keep pointers to projected points in image
	multi_dimensional_array<Point*, 2> image({cam.image_width(), cam.image_height()}, nullptr);
	for(Point* p = super::begin_; p < super::end_; ++p) {
		if(! p->valid()) continue;
		
		float z;
		auto ic = cam.to_image(*p, z);
		if(! cam.in_bounds(ic)) continue;
		
		auto& old_ptr = image[ic];
		if(old_ptr == nullptr || cam.depth_sq(*old_ptr) > cam.depth_sq(*p))
			old_ptr = p;
	}
	
	// Invalidate all points, and then revalidate those retained for image cells
	#pragma omp parallel for
	for(Point* p = super::begin_; p < super::end_; ++p) p->invalidate();

	#pragma omp parallel for
	for(auto it = image.begin_raw(); it < image.end_raw(); ++it) {
		Point* p = *it;
		if(p) p->revalidate();
	}

	// If not in invalidate mode: Need to erase invalid from pc
	if(! invalidate) erase_invalid_points();
}


template<typename Point, typename Allocator>
void unorganized_point_cloud<Point, Allocator>::add_random_noise_in_box(std::size_t amount, const bounding_box& box) {
	if(amount > super::remaining_capacity())
		throw std::logic_error("Not enough capacity for requested amound of additional points.");

	Point* np = super::end_;
	while(amount--) {
		Eigen::Vector3f new_pt;
		for(std::ptrdiff_t i = 0; i < 3; ++i)
			new_pt[i] = random_real(box.origin[i], box.extremity[i]);
		
		*(np++) = Point(new_pt);
	}
	
	super::end_ = np;
}





template<typename Point, typename Allocator> template<typename Other_point>
const Point& unorganized_point_cloud<Point, Allocator>::
closest_point(const Other_point& query, float accepting_distance, float rejecting_distance) const {
	auto it = find_closest_point(query, super::cbegin(), super::cend(), accepting_distance);
	if(it == super::cend()) return super::invalid_point_();
	else return *it;
}


}