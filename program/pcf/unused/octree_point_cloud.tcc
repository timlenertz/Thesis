#include <utility>
#include <Eigen/StdVector>

namespace pcf {

template<typename Point, typename Allocator> template<typename Other_cloud>
octree_point_cloud<Point, Allocator>::octree_point_cloud(Other_cloud&& pc, std::size_t leaf_cap, const Allocator& alloc) :
super(std::forward<Other_cloud>(pc), alloc), leaf_capacity_(leaf_cap), root_node_(super::full_segment_()) {
	build_tree_();
}


template<typename Point, typename Allocator> template<typename Other_cloud>
octree_point_cloud<Point, Allocator>::octree_point_cloud(Other_cloud&& pc, std::size_t leaf_cap) :
super(std::forward<Other_cloud>(pc)), leaf_capacity_(leaf_cap), root_node_(super::full_segment_()) {
	build_tree_();
}


template<typename Point, typename Allocator>
void octree_point_cloud<Point, Allocator>::build_tree_() {
	// Setup root cuboid, enclosing whole point cloud
	root_cuboid_ = super::bounding_cuboid(0.1); // TODO choose good ep

	// Construct Octree by breath-first descent.
	// Creates node tree, and segments point cloud in-place.
	using node_with_cuboid = std::pair<
		std::reference_wrapper<node>,
		node_cuboid
	>;
	std::vector<node_with_cuboid> todo, next_todo; // nodes to split at current and next iteration
	todo.emplace_back(root_node_, root_cuboid_);
	
	while(todo.size()) {
		// todo contains set of nodes at same level.

		// Segments for nodes in todo don't overlap
		// Can split them in simultaneously		
		#pragma omp parallel
		{
			std::vector<node_with_cuboid> next_todo_part;
		
			#pragma omp for
			for(auto it = todo.cbegin(); it < todo.cend(); ++it) {
				node& nd = it->first;
				const node_cuboid& cub = it->second;
				
				if(nd.size() < leaf_capacity_) continue;
		
				// Split the node
				// Rearranges data in node's segment into subsegments for children
				// and instanciates node's child nodes
				split_node_(nd, cub);
		
				// Schedule child nodes for next iteration
				for(std::ptrdiff_t i = 0; i < 8; ++i) if(nd.has_child(i)) {
					node& cnd = nd.child(i);
					next_todo_part.emplace_back(cnd, cub.child_cuboid(i));
				}
			}
			
			#pragma omp critical
			{ next_todo.insert(next_todo.end(), next_todo_part.cbegin(), next_todo_part.cend()); }
		}
		
		todo = next_todo;
		next_todo.clear();
	}
}


template<typename Point, typename Allocator>
void octree_point_cloud<Point, Allocator>::verify_(const node& nd, const node_cuboid& cub) const {
	// Leaves may not have more points than leaf_capacity_
	if(nd.is_leaf() && nd.size() > leaf_capacity_) throw std::logic_error("Leaf has more children than leaf capacity.");
	
	// Node must not be empty
	if(nd.size() == 0) throw std::logic_error("Node must not be empty.");
	
	// Make sure all points in segment belong to cuboid
	for(const Point* p = nd.begin(); p < nd.end(); ++p)
		if(! cub.contains(*p)) throw std::logic_error("Node contains point which is not in its cuboid.");
		
	// Recursively check children
	for(std::ptrdiff_t i = 0; i < 8; ++i) if(nd.has_child(i)) {
		const node& cnd = nd.child(i);
		const node_cuboid& ccub = cub.child_cuboid(i);
		verify_(cnd, ccub);
	}
}


template<typename Point, typename Allocator>
void octree_point_cloud<Point, Allocator>::split_node_(node& nd, const node_cuboid& cub) {
	auto idx = [&cub](const Point& p) -> std::ptrdiff_t {
		return cub.child_for_point(p);
	};
	std::vector<segment> child_segments = super::make_segments_(idx, 8, nd.begin(), nd.end());
	assert(child_segments.back().end() == nd.end());
	
	for(std::ptrdiff_t i = 0; i < 8; ++i)
		if(child_segments[i].size()) nd.create_child(i, child_segments[i]);
}



template<typename Point, typename Allocator>
auto octree_point_cloud<Point, Allocator>::node_cuboid::child_cuboid(std::ptrdiff_t i) const -> node_cuboid {
	Eigen::Vector3f c = center();
	bool x = (i % 2);
	bool y = (i % 4 > 1);
	bool z = (i >= 4);
	
	Eigen::Vector3f corigin(
		x ? c[0] : origin[0],
		y ? c[1] : origin[1],
		z ? c[2] : origin[2]
	);
	Eigen::Vector3f cextremity(
		x ? extremity[0] : c[0],
		y ? extremity[1] : c[1],
		z ? extremity[2] : c[2]
	);
	return node_cuboid(corigin, cextremity);
}


template<typename Point, typename Allocator> template<typename Other_point>
std::ptrdiff_t octree_point_cloud<Point, Allocator>::node_cuboid::child_for_point(const Other_point& p) const {
	//assert(contains(p));
	
	if(! contains(p)) {
		std::cout << p << " not in" << *this << std::endl;
	}
	
	Eigen::Vector3f c = center();
	std::ptrdiff_t i = 0;
	if(p[0] >= c[0]) i += 1;
	if(p[1] >= c[1]) i += 2;
	if(p[2] >= c[2]) i += 4;
	return i;
}


template<typename Point, typename Allocator> template<typename Other_point>
auto octree_point_cloud<Point, Allocator>::node_containing_point_(const Other_point& pt, const node& nd, const node_cuboid& cub, std::size_t max_depth) const -> const node& {
	assert(cub.contains(pt));
	
	if(max_depth == 1 || nd.is_leaf()) {
		return nd;
	} else {
		std::ptrdiff_t i = cub.child_for_point(pt);
		if(! nd.has_child(i)) return nd;
		else return node_containing_point_(pt, nd.child(i), cub.child_cuboid(i), max_depth - 1);
	}
}


template<typename Point, typename Allocator> template<typename Other_point>
const Point& octree_point_cloud<Point, Allocator>::find_closest_point(const Other_point& from) const {
	const node& nd = node_containing_point_(from, root_node_, root_cuboid_);
	return super::find_closest_point(from, euclidian_distance_sq, nd.begin(), nd.end());
}



template<typename Point, typename Allocator>
bool octree_point_cloud<Point, Allocator>::node::is_leaf() const {
	for(const child_ptr& c : children_) if(c) return false;
	return true;
}


template<typename Point, typename Allocator>
auto octree_point_cloud<Point, Allocator>::node::create_child(std::ptrdiff_t i, const segment& seg) -> node& {
	node* cnd = new node(seg);
	children_[i].reset(cnd);
	return *cnd;
}


}
