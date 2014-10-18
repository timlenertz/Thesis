#include <utility>
#include <Eigen/StdVector>

namespace pcf {

template<typename Point, typename Allocator> template<typename Other_cloud>
kdtree_point_cloud<Point, Allocator>::kdtree_point_cloud(Other_cloud&& pc, std::size_t leaf_cap, const Allocator& alloc) :
super(std::forward<Other_cloud>(pc), true, alloc), leaf_capacity_(leaf_cap), root_node_(super::full_segment_()) {
	build_tree_();
}

template<typename Point, typename Allocator> template<typename Other_cloud>
kdtree_point_cloud<Point, Allocator>::kdtree_point_cloud(Other_cloud&& pc, std::size_t leaf_cap) :
super(std::forward<Other_cloud>(pc), true), leaf_capacity_(leaf_cap), root_node_(super::full_segment_()) {
	build_tree_();
}


template<typename Point, typename Allocator>
void kdtree_point_cloud<Point, Allocator>::build_tree_() {
	// Setup root cuboid, enclosing whole point cloud
	root_cuboid_ = super::bounding_cuboid(0.1); // TODO choose good ep

	// Construct Kdtree by breath-first descent.
	// Creates node tree, and segments point cloud in-place.
	using node_with_cuboid = std::pair<
		std::reference_wrapper<node>,
		node_cuboid
	>;
	std::vector<node_with_cuboid> todo, next_todo; // nodes to split at current and next iteration
	todo.emplace_back(root_node_, root_cuboid_);
	
	int i = 0;
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
				next_todo_part.emplace_back(nd.left(), cub.child_cuboid(false, nd));
				next_todo_part.emplace_back(nd.right(), cub.child_cuboid(true, nd));
			}
			
			#pragma omp critical
			{ next_todo.insert(next_todo.end(), next_todo_part.cbegin(), next_todo_part.cend()); }
		}
		
		todo = next_todo;
		next_todo.clear();
	}
}


template<typename Point, typename Allocator>
void kdtree_point_cloud<Point, Allocator>::verify_(const node& nd, const node_cuboid& cub) const {
	// Leaves may not have more points than leaf_capacity_
	if(nd.is_leaf() && nd.size() > leaf_capacity_) throw std::logic_error("Leaf has more children than leaf capacity.");
	
	// Node must not be empty
	if(nd.size() == 0) throw std::logic_error("Node must not be empty.");
	
	// Make sure all points in segment belong to cuboid
	for(const Point* p = nd.start(); p < nd.end(); ++p)
		if(! cub.contains(*p)) //throw std::logic_error("Node contains point which is not in its cuboid.");
	{ std::cout << *p << " not in " << cub << std::endl; }
	// Recursively check children
	if(! nd.is_leaf()) {
		verify_(nd.left(), cub.child_cuboid(true, nd));
		verify_(nd.right(), cub.child_cuboid(false, nd));
	}
}


template<typename Point, typename Allocator>
void kdtree_point_cloud<Point, Allocator>::split_node_(node& nd, const node_cuboid& cub) {
	auto cmp = [&cub](const Point& a, const Point& b) -> bool {
		return (a[cub.orientation] < b[cub.orientation]);
	};
	
	super::sort_points(cmp, nd.start(), nd.end());
	Point* median = nd.start() + nd.size()/2;
	
	nd.create_children(median, cub);
}



template<typename Point, typename Allocator>
auto kdtree_point_cloud<Point, Allocator>::node_cuboid::child_cuboid(bool left, const node& nd) const -> node_cuboid {
	node_cuboid ccub(*this);
	
	if(left) ccub.extremity[orientation] = nd.split_plane();
	else ccub.origin[orientation] = nd.split_plane();
	
	ccub.orientation = (orientation + 1) % 3;
	
	return ccub;
}


template<typename Point, typename Allocator> template<typename Other_point>
bool kdtree_point_cloud<Point, Allocator>::node_cuboid::child_for_point(const Other_point& p, const node& nd) const {
	assert(contains(p));
	return (p[orientation] < nd.split_plane());
}


template<typename Point, typename Allocator> template<typename Other_point>
auto kdtree_point_cloud<Point, Allocator>::node_containing_point_(const Other_point& pt, const node& nd, const node_cuboid& cub, std::size_t max_depth) const -> const node& {
	assert(cub.contains(pt));
	
	if(max_depth == 1 || nd.is_leaf()) {
		return nd;
	} else {
		bool left = cub.child_for_point(pt, nd);
		return node_containing_point_(
			pt,
			left ? nd.left() : nd.right(),
			cub.child_cuboid(left, nd),
			max_depth - 1
		);
	}
}


template<typename Point, typename Allocator> template<typename Other_point>
const Point& kdtree_point_cloud<Point, Allocator>::find_closest_point(const Other_point& from) const {
	const node& nd = node_containing_point_(from, root_node_, root_cuboid_);
	return super::find_closest_point_in_segment_(from, nd, euclidian_distance_sq);
}



template<typename Point, typename Allocator>
bool kdtree_point_cloud<Point, Allocator>::node::is_leaf() const {
	return ! bool(left_);
}


template<typename Point, typename Allocator>
void kdtree_point_cloud<Point, Allocator>::node::create_children(Point* split, const node_cuboid& cub) {
	split_plane_ = (*split)[cub.orientation];
	
	node* l = new node(super::start(), split);
	node* r = new node(split, super::end());
	
	left_.reset(l);
	right_.reset(r);
}


}
