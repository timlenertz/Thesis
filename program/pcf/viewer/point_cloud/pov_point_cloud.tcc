#include <utility>
#include <queue>
#include <vector>
#include <functional>
#include <cstring>

namespace pcf {

static const leaf_capacity_ = 1000;

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

}


template<typename Point, typename Allocator>
std::size_t pov_point_cloud<Point, Allocator>::
extract(Point* buffer, std::size_t capacity, const camera& cam) const {
	// 1st step: Put set of non-overlapping set of nodes that are inside of frustum in queue.
	// These will be extracted, with a given level of downsampling for each of them; without
	// further descending into the nodes.

	auto cmp = [&cam](const const_node_handle& a, const const_node_handle& b) {
		float da = distance_sq(cam.camera_pose().position, a.attr().center_of_mass);
		float db = distance_sq(cam.camera_pose().position, b.attr().center_of_mass);
		return (da > db);
	};

	// Priority queue ordered by distance of nodes' center of mass to camera origin
	std::priority_queue<const_node_handle, std::vector<const_node_handle>, decltype(cmp)> queue(cmp);
	std::size_t total_size = 0;
	float max_d = 0;
	
	std::function<void(const const_node_handle& nd)> ins;
	ins = [&queue, &cam, &total_size, &ins](const const_node_handle& nd) {
		frustum::intersection inter = fr.contains(nd.box());
		if(inter == frustum::outside_frustum) {
			return;
		} else if(nd.is_leaf() || inter == frustum::inside_frustum) {
			total_size += nd.size();
			queue.push(nd);
			float d = distance(cam.camera_pose().position, nd.attr().center_of_mass);
			if(d > max_d) max_d = d;
		} else {
			for(std::ptrdiff_t ci = 0; ci != Traits::number_of_children; ++ci)
				if(nd.has_child(ci)) ins(nd.child(i))
		}		
	};
	ins(root());
	
	
	// 2nd step: Extract points in these nodes
	std::size_t extracted = 0;
	while(!queue.empty() && extracted < capacity) {
		const_node_handle nd = queue.top();
		queue.pop();
		
		float d = distance(cam.camera_pose().position, nd.attr().center_of_mass);
		std::size_t n = (nd.size() * (max_d - d)) / max_d;
		extracted += extract_points_(buffer + extracted, capacity - extracted, nd, n);
	}
	
	
	return extracted;
}





template<typename Point, typename Allocator>
std::size_t pov_point_cloud<Point, Allocator>::
extract_points_(Point* buffer, std::size_t capacity, const const_node_handle& nd, std::size_t max_number) const {
	std::size_t n = max_number;
	if(max_number > capacity) n = capacity;
	std::memcpy( (void*)buffer, (const void*)nd.seg().data(), n * sizeof(Point) );
	return n;
}





}