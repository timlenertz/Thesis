#include <utility>
#include <queue>
#include <vector>
#include <functional>
#include <cstring>
#include <algorithm>
#include "../../pcf/geometry/frustum.h"
#include "../../pcf/geometry/camera.h"
#include "../../pcf/util/random.h"
#include "../../pcf/point_cloud_algorithm.h"

#include <iostream>

namespace pcf {

template<typename Point, typename Allocator> template<typename Other_cloud>
pov_point_cloud<Point, Allocator>::pov_point_cloud(Other_cloud&& pc, const Allocator& alloc) :
super(std::forward<Other_cloud>(pc), compute_leaf_capacity_(pc), alloc) {
	prepare_tree_();
}


template<typename Point, typename Allocator> template<typename Other_cloud>
std::size_t pov_point_cloud<Point, Allocator>::compute_leaf_capacity_(const Other_cloud& pc) {
	std::size_t cap = pc.size() / 100000;
	if(cap < 1000) cap = 1000;
	return cap;
}




template<typename Point, typename Allocator>
void pov_point_cloud<Point, Allocator>::prepare_tree_() {
	std::function<void(const node_handle& nd)> shuffle_leaves;
	shuffle_leaves = [this, &shuffle_leaves](const node_handle& nd) {
		if(nd.is_leaf()) {
			auto seg = nd.seg();
			#pragma omp task
			std::shuffle(seg.begin(), seg.end(), get_random_generator());
		} else {
			for(std::ptrdiff_t ci = 0; ci != 8; ++ci)
				if(nd.has_child(ci)) shuffle_leaves(nd.child(ci));
		}
	};
	
	#pragma omp parallel
	{
		shuffle_leaves(super::root());
	}
}


template<typename Point, typename Allocator>
std::size_t pov_point_cloud<Point, Allocator>::
extract(Point* buffer, std::size_t capacity, const camera& cam) const {
	// 1st step: Put set of non-overlapping set of nodes that are inside of frustum in queue.
	// These will be extracted, with a given level of downsampling for each of them; without
	// further descending into the nodes.

	auto cmp = [&cam](const const_node_handle& a, const const_node_handle& b) {
		float da = distance_sq(cam.get_pose().position, a.attr().center_of_mass);
		float db = distance_sq(cam.get_pose().position, b.attr().center_of_mass);
		return (da > db);
	};

	// Priority queue ordered by distance of nodes' center of mass to camera origin
	std::priority_queue<const_node_handle, std::vector<const_node_handle>, decltype(cmp)> queue(cmp);
	std::size_t total_size = 0;
	
	
	frustum_planes fr = cam.viewing_frustum().planes();
		
	std::function<void(const const_node_handle& nd)> ins;
	ins = [&](const const_node_handle& nd) {
		frustum::intersection inter = frustum::contains(fr, nd.box());
		if(inter == frustum::outside_frustum) {
			return;
		} else if(nd.is_leaf() || (inter == frustum::inside_frustum && nd.box().side_length() < extra_split_side_length_)) {
			total_size += nd.size();
			queue.push(nd);

		} else {
			for(std::ptrdiff_t ci = 0; ci != 8; ++ci)
				if(nd.has_child(ci)) ins(nd.child(ci));
		}		
	};
	//ins(super::root());
	queue.push(super::root());
	total_size = super::root().size();

	// 2nd step: Extract points in these nodes
	bool downsample = (total_size > capacity);
	std::size_t extracted = 0, remaining_capacity = capacity, remaining_size = total_size;
	Point* buf = buffer;
	while(!queue.empty() && remaining_capacity) {
		const_node_handle nd = queue.top();
		queue.pop();
		
		std::size_t n = nd.size();
		if(downsample) {		
			n = (nd.size() * remaining_capacity) / remaining_size;
		
			if(n > nd.size()) n = nd.size();
			if(n > remaining_capacity) n = remaining_capacity;
		}
		
		extract_points_(buf, n, nd);
				
		buf += n;
		extracted += n;
		remaining_capacity -= n;
		remaining_size -= nd.size();
	}
			
	return extracted;
}



template<typename Point, typename Allocator>
void pov_point_cloud<Point, Allocator>::
extract_points_(Point* buffer, std::size_t n, const const_node_handle& nd) const {
	// n is guaranteed to be smaller of equal to node size and to remaining buffer capacity
	
	if(n == 0) return;
	
	std::size_t ndsize = nd.size();
	assert(n <= ndsize);
		
	if(n == ndsize || nd.is_leaf()) {
		// Copy all points
		std::memcpy( (void*)buffer, (const void*)nd.seg().data(), n * sizeof(Point) );
	
	/*	
	} else if(ndsize % n < skip_tolerance) {
		// Copy every k-th point (k constant).
		std::ptrdiff_t k = ndsize / n;
		
		Point* out = buffer;
		const Point* in = nd.seg().data();
		in += (ndsize % n) / 2;

		for(std::ptrdiff_t i = 0; i < n; ++i) {		
			*(out++) = *in;
			in += k;
		}
	*/
	
	} else {
		// Apply Bresenham's line algorithm to select well distributed subset of points
		Point* out = buffer;
		const Point* in = nd.seg().data();

		std::ptrdiff_t d = 2*std::ptrdiff_t(n) - std::ptrdiff_t(ndsize);

		std::ptrdiff_t dd1 = 2*std::ptrdiff_t(n) - 2*std::ptrdiff_t(ndsize);
		std::ptrdiff_t dd2 = 2*std::ptrdiff_t(n);
				
		Point* out_end = buffer + n;
		for(std::ptrdiff_t i = 1; i < ndsize && out != out_end; ++i) {		
			if(d > 0) {
				*(out++) = *in;
				d += dd1;
			} else {
				d += dd2;
			}
			++in;
		}
	}
}



}