#include <utility>
#include <cstring>
#include <algorithm>
#include <stdexcept>
#include "../util/random.h"
#include "../util/coordinates.h"

namespace pcf {


template<typename Point, typename Allocator> template<typename Filter>
void filter_point_cloud<Point, Allocator>::filter(Filter filt, bool par) {
	filt.reset();

	Point* np = super::begin_;

	#pragma omp parallel for if(par)
	for(Point* p = super::begin_; p < super::end_; ++p) {
		if(! p->valid()) continue;
		bool accept = filt(*p);
		if(! accept) p->invalidate();
	}
}



template<typename Point, typename Allocator>
void filter_point_cloud<Point, Allocator>::downsample_random(float ratio) {
	random_generator& rng = get_random_generator();
	
	const std::size_t total = super::number_of_valid_points();
	const std::size_t expected = (1.0f - ratio) * total;
	std::size_t left = total;
	std::size_t needed = expected;
	
	for(Point* p = super::begin_; p < super::end_; ++p) {
		if(! super::all_valid_) if(! p->valid()) continue;
		std::uniform_int_distribution<std::size_t> dist(0, left - 1);
		if(dist(rng) < needed) {
			p->invalidate();
			--needed;
		}
		--left;
	}
}



template<typename Point, typename Allocator> template<typename Field>
void filter_point_cloud<Point, Allocator>::downsample_random_field(const Field& field) {
	#pragma omp parallel for
	for(Point* p = super::begin_; p < super::end_; ++p) {
		if(! p->valid()) continue;
		float r = random_real(0.0f, 1.0f);
		float f = field(p->coordinates());
		if(r < f) p->invalidate();
	}
}


template<typename Point, typename Allocator>
void filter_point_cloud<Point, Allocator>::downsample_grid(float cell_len, bool keep_first) {
	// Create 3D array storing closest point to center of each cell
	// or nullptr if no point in that cell
	bounding_box b = super::box();
	size_3dim sz;
	for(std::ptrdiff_t i = 0; i < 3; ++i) sz[i] = std::ceil(b.side_lengths()[i] / cell_len);
	
	array_3dim<Point*> cells(sz, nullptr);
	
	auto cell_center = [&b, &cell_len](const index_3dim& mi) -> Eigen::Vector3f {
		Eigen::Vector3f center;
		for(auto i = 0; i < 3; ++i) center[i] = mi[i] * cell_len;
		return (center + b.origin);
	};
	
	// For each point: Retain closest point to center of each cell
	#pragma omp parallel for
	for(Point* p = super::begin_; p < super::end_; ++p) {
		index_3dim mi;
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
}


template<typename Point, typename Allocator> template<typename Image_camera>
void filter_point_cloud<Point, Allocator>::downsample_projection(const Image_camera& cam) {
	// Image with pointers to points to keep
	multi_dimensional_array<Point*, 2> points_buffer({ cam.image_width(), cam.image_height() }, nullptr);
	
	// Project, and keep closest points
	#pragma omp parallel for
	for(auto it = super::begin(); it < super::end(); ++it) {
		auto& p = *it;
		if(! p.valid()) continue;
		
		float z;
		auto ic = cam.to_image(p, z);
		if(! cam.in_bounds(ic)) continue;

		auto& old_point = points_buffer[{ic[0], ic[1]}];
		auto old_z = cam.depth(p);
		if(z < old_z) {
			old_point = &p;
		}
	}

	// Invalidate all points
	for(Point& pt : *this) pt.invalidate();
	
	// Revalidate the kept points
	for(Point* pt : points_buffer)
		if(pt != nullptr) pt->revalidate();
}



template<typename Point, typename Allocator> template<typename Camera>
void filter_point_cloud<Point, Allocator>::erase_invisible_points(const Camera& cam) {
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
}


template<typename Point, typename Allocator> template<typename Mask>
void filter_point_cloud<Point, Allocator>::filter_mask(const Mask& mask, bool inverse) {
	auto mit = mask.cbegin();
	for(Point& pt : *this) {
		if(mit == mask.cend()) break;
		else if(*(mit++) != inverse) pt.invalidate();
	}
}

template<typename Point, typename Allocator>
std::vector<bool> filter_point_cloud<Point, Allocator>::valid_points_mask() const {
	std::vector<bool> mask(super::size());
	auto mit = mask.begin();
	for(const Point& pt : *this) *(++mit) = pt.valid();
	return mask;
}


}