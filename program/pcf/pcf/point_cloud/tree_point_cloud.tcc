#include <set>
#include <iterator>

namespace pcf {

template<typename Traits, typename Point, typename Allocator> template<typename Other_cloud>
tree_point_cloud<Traits, Point, Allocator>::tree_point_cloud(Other_cloud&& pc, std::size_t leaf_cap, const Allocator& alloc) :
super(std::forward<Other_cloud>(pc), true, alloc), leaf_capacity_(leaf_cap), root_node_() {
	build_tree_();
}


template<typename Traits, typename Point, typename Allocator> template<typename Other_cloud>
tree_point_cloud<Traits, Point, Allocator>::tree_point_cloud(Other_cloud&& pc, std::size_t leaf_cap) :
super(std::forward<Other_cloud>(pc), true), leaf_capacity_(leaf_cap), root_node_(*this) {
	build_tree_();
}


template<typename Traits, typename Point, typename Allocator>
auto tree_point_cloud<Traits, Point, Allocator>::root() -> node_handle {
	return node_handle(root_node_, root_box_, 0);
}


template<typename Traits, typename Point, typename Allocator>
auto tree_point_cloud<Traits, Point, Allocator>::root() const -> const_node_handle {
	return const_node_handle(root_node_, root_box_, 0);
}


template<typename Traits, typename Point, typename Allocator>
void tree_point_cloud<Traits, Point, Allocator>::build_tree_() {
	using node_handle_list = std::vector<node_handle>;

	root_box_ = Traits::root_box(*this);

	// Construct tree by breath-first descent.
	// Creates node tree, and segment point cloud in-place.
	node_handle_list todo, next_todo; // nodes to split at current and next iteration
	todo.push_back(root());
	
	std::size_t depth = 0;
	while(todo.size() && depth != Traits::maximal_depth) {
		// todo contains set of nodes at same level.

		// Segments for nodes in todo don't overlap
		// Can split them simultaneously		
		#pragma omp parallel
		{
			node_handle_list next_todo_part;
		
			#pragma omp for
			for(auto it = todo.begin(); it < todo.end(); ++it) {								
				if(it->nd().seg.size() <= leaf_capacity_) continue;
		
				// Split the node
				// Rearranges data in node's segment into subsegments for children
				auto child_segments = Traits::split_node(it->nd().seg, it->box(), it->attr(), it->depth());
				
				assert(child_segments[0].begin() == it->nd().seg.begin());
				assert(child_segments[Traits::number_of_children - 1].end() == it->nd().seg.end());

				// Create child nodes, and schedule for next iteration
				for(std::ptrdiff_t i = 0; i < Traits::number_of_children; ++i) {
					auto& seg = child_segments[i];
					if(! seg.empty()) {
						//for(Point& p : seg) mark_point(p, std::rand());
					
						it->make_child(i, seg);
						next_todo_part.push_back(it->child(i));
					}
				}
			}

			#pragma omp critical
			{
				next_todo.insert(next_todo.end(), next_todo_part.cbegin(), next_todo_part.cend());
			}
		}
		
		todo = next_todo;
		next_todo.clear();
		++depth;
	}
}


template<typename Traits, typename Point, typename Allocator>
bool tree_point_cloud<Traits, Point, Allocator>::verify_(const const_node_handle& an) const {
	for(const Point& p : an->seg) if(! an.box().contains(p)) return false;
	
	for(std::ptrdiff_t i = 0; i < Traits::number_of_children; ++i) {
		if(! an.has_child(i)) continue;

		bool ok = verify_( an.child(i) );
		if(! ok) return false;
	}
	
	return true;
}



}