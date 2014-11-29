#include <utility>
#include <queue>
#include <vector>
#include <functional>
#include <cstring>
#include <algorithm>
#include "../../pcf/geometry/frustum.h"
#include "../../pcf/geometry/camera.h"
#include "../../pcf/util/random.h"

namespace pcf {

template<typename Point, typename Allocator> template<typename Other_cloud>
pov_point_cloud<Point, Allocator>::pov_point_cloud(Other_cloud&& pc, const Allocator& alloc) :
super(std::forward<Other_cloud>(pc), leaf_capacity_, alloc) {
	prepare_tree_();
}


template<typename Point, typename Allocator> template<typename Other_cloud>
pov_point_cloud<Point, Allocator>::pov_point_cloud(Other_cloud&& pc) :
super(std::forward<Other_cloud>(pc), leaf_capacity_) {
	prepare_tree_();
}


template<typename Point, typename Allocator>
void pov_point_cloud<Point, Allocator>::prepare_tree_() {
	std::function<void(const node_handle& nd)> shuffle_leaves;
	shuffle_leaves = [this, &shuffle_leaves](const node_handle& nd) {
		if(nd.is_leaf()) {
			auto seg = nd.seg();
			std::shuffle(seg.begin(), seg.end(), get_random_generator());
		} else {
			for(std::ptrdiff_t ci = 0; ci != 8; ++ci)
				if(nd.has_child(ci)) shuffle_leaves(nd.child(ci));
		}
	};
	shuffle_leaves(super::root());
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
	
	auto node_w = [&cam](const const_node_handle& nd)->float {
		float d = distance(cam.get_pose().position, nd.attr().center_of_mass);
		return 1.0f / (std::pow(d, 0.2f) + 1.0f);
	};

	// Priority queue ordered by distance of nodes' center of mass to camera origin
	std::priority_queue<const_node_handle, std::vector<const_node_handle>, decltype(cmp)> queue(cmp);
	std::size_t total_size = 0;
	
	float w_sum = 0;
	
	frustum fr = cam.viewing_frustum();
	
	std::function<void(const const_node_handle& nd)> ins;
	ins = [&](const const_node_handle& nd) {
		frustum::intersection inter = fr.contains(nd.box());
		if(inter == frustum::outside_frustum) {
			return;
		} else if(nd.is_leaf() || (inter == frustum::inside_frustum && nd.box().side_length() < extra_split_side_length_)) {
			total_size += nd.size();
			queue.push(nd);
			
			float w = node_w(nd);
			w_sum += w;

		} else {
			for(std::ptrdiff_t ci = 0; ci != 8; ++ci)
				if(nd.has_child(ci)) ins(nd.child(ci));
		}		
	};
	ins(super::root());
	

	// 2nd step: Extract points in these nodes
	bool downsample = (total_size > capacity);
	std::size_t extracted = 0, remaining_capacity = capacity;
	Point* buf = buffer;
	while(!queue.empty() && extracted < capacity) {
		const_node_handle nd = queue.top();
		queue.pop();
		
		float w = node_w(nd);

		std::size_t n = nd.size();
		if(downsample) {
			float distance_weight = w / w_sum;
			
		
			n = (w * remaining_capacity) / w_sum;

			if(n > nd.size()) n = nd.size();
			if(n > remaining_capacity) n = remaining_capacity;
		}

		extract_points_(buf, n, nd);
		
		buf += n;
		extracted += n;
		remaining_capacity -= n;
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
	
	std::size_t skip_tolerance = ndsize / 9;
		
	if(n == ndsize) {
		// Copy all points
		std::memcpy( (void*)buffer, (const void*)nd.seg().data(), n * sizeof(Point) );
		
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