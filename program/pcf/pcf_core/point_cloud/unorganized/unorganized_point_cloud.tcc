#include <utility>
#include <cstring>
#include <algorithm>
#include <stdexcept>
#include "../../io/point_cloud_importer.h"
#include "../../util/random.h"

namespace pcf {

template<typename Point, typename Allocator>
unorganized_point_cloud<Point, Allocator>::unorganized_point_cloud(std::size_t sz, std::size_t cap, const Allocator& alloc) :
	super( (cap != 0) ? cap : sz, alloc)
{
	if(sz > super::capacity()) throw std::invalid_argument("Size larger than capacity.");
	super::resize_(sz);
	super::initialize_();
}


template<typename Point, typename Allocator>
unorganized_point_cloud<Point, Allocator>::unorganized_point_cloud(point_cloud_importer& imp, const Allocator& alloc) :
	super(imp.size(), alloc)
{
	if(imp.has_camera_pose())
		super::set_relative_pose( imp.camera_pose() );

	std::size_t n = super::capacity();
	super::resize_(n);
	
	imp.rewind();
	imp.read(super::begin_, n);
}


template<typename Point, typename Allocator>
void unorganized_point_cloud<Point, Allocator>::apply_transformation(const Eigen::Affine3f& t) {	
	#pragma omp parallel for
	for(auto p = super::begin(); p < super::end(); ++p)
		if(p->valid()) p->apply_transformation(t);
}





template<typename Point, typename Allocator>
void unorganized_point_cloud<Point, Allocator>::shuffle() {
	std::shuffle(super::begin(), super::end(), get_random_generator());
}


template<typename Point, typename Allocator> template<typename Other_iterator>
auto unorganized_point_cloud<Point, Allocator>::insert(typename super::iterator pos_start, Other_iterator begin, Other_iterator end)
-> typename super::iterator {
	typename super::iterator pos = pos_start;
	for(auto it = begin; it < end; ++it) {
		if(! it->valid()) continue;
		if(pos == super::end()) throw std::invalid_argument("Cannot insert points past the end of the point cloud.");
		*(pos++) = *(it);
	}
	return pos;
}



template<typename Point, typename Allocator>
void unorganized_point_cloud<Point, Allocator>::erase_invalid_points() {	
	super::end_ = std::partition(super::begin(), super::end(), [](const Point& p) {
		return p.valid();
	});
}


template<typename Point, typename Allocator> template<typename Distribution>
void unorganized_point_cloud<Point, Allocator>::randomly_displace_points(Distribution dist) {
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
void unorganized_point_cloud<Point, Allocator>::randomly_displace_points_on_local_surface(Distribution dist) {
	random_generator& rng = get_random_generator();

	for(Point* p = super::begin_; p < super::end_; ++p) {
		if(! p->valid()) continue;

		const Eigen::Vector3f& n = p->get_normal();
		
		Eigen::Vector3f d(dist(rng), dist(rng), 0.0);
		d[2] = (d[0]*n[0] + d[1]*n[1]) / n[2];
		
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


template<typename Point, typename Allocator>
void unorganized_point_cloud<Point, Allocator>::apply_pose() {	
	const Eigen::Affine3f& t = super::relative_pose().transformation_to_world();
	apply_transformation(t);
	super::set_relative_pose(pose());
}



template<typename Point, typename Allocator> template<typename Condition_func, typename Callback_func>
void unorganized_point_cloud<Point, Allocator>::nearest_neighbors(std::size_t k, Condition_func cond, Callback_func callback, bool par) const {
	#pragma omp parallel for if(par)
	for(auto it = super::begin(); it < super::end(); ++it) {
		const Point& ref = *it;
		if(! cond(ref)) continue;
		
		auto closest = find_nearest_neighbors(ref, super::begin(), super::end(), k);
		callback(ref, closest.begin(), closest.end());
	}
}



template<typename Point, typename Allocator> template<typename Condition_func, typename Callback_func>
void unorganized_point_cloud<Point, Allocator>::nearest_neighbors(std::size_t k, Condition_func cond, Callback_func callback, bool par) {
	#pragma omp parallel for if(par)
	for(auto it = super::begin(); it < super::end(); ++it) {
		Point& ref = *it;
		if(! cond(ref)) continue;
		
		auto closest = find_nearest_neighbors(ref, super::begin(), super::end(), k);
		callback(ref, closest.begin(), closest.end());
	}
}



}