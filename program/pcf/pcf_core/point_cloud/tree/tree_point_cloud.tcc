#include <set>
#include <iterator>
#include <utility>
#include "node_handle.h"
#include "../../util/memory.h"

#include <iostream>

namespace pcf {

template<typename Traits, typename Point, typename Allocator>
std::size_t tree_point_cloud<Traits, Point, Allocator>::compute_leaf_capacity_(std::size_t requested, bool round_up) {
	if(requested == 0) requested = system_page_size / sizeof(Point);
	if(round_up) requested = round_up_to_fit_system_page_size<Point>(requested);
	return requested;
}


template<typename Traits, typename Point, typename Allocator> template<typename Other_cloud>
tree_point_cloud<Traits, Point, Allocator>::tree_point_cloud(const Other_cloud& pc, std::size_t leaf_cap, bool round_up, const Allocator& alloc) :
super(pc, true, 0, alloc),
leaf_capacity_( compute_leaf_capacity_(leaf_cap, round_up) ),
root_node_(super::full_segment()) {
	build_tree_();
}


template<typename Traits, typename Point, typename Allocator>
tree_point_cloud<Traits, Point, Allocator>::tree_point_cloud(super&& pc, std::size_t leaf_cap, bool round_up) :
super(std::move(pc), true),
leaf_capacity_( compute_leaf_capacity_(leaf_cap, round_up) ),
root_node_(super::full_segment()) {
	build_tree_();
}



template<typename Traits, typename Point, typename Allocator>
tree_point_cloud<Traits, Point, Allocator>::tree_point_cloud(tree_point_cloud&& pc) :
super(std::move(pc), true),
leaf_capacity_(pc.leaf_capacity_),
root_node_(std::move(pc.root_node_)),
root_box_(pc.root_box_) { }



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

	root_box_ = Traits::root_box(super::full_segment());

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
				it->attr() = Traits::initialize_node_attributes(it->seg(), it->box(), depth);
													
				if(it->size() <= leaf_capacity_) continue;
		
				// Split the node
				// Rearranges data in node's segment into subsegments for children
				auto child_segments = Traits::split_node(it->seg(), it->box(), it->attr(), it->depth());
				
				assert(child_segments[0].begin() == it->begin());
				assert(child_segments[Traits::number_of_children - 1].end() == it->end());

				// Create child nodes, and schedule for next iteration
				for(std::ptrdiff_t i = 0; i < Traits::number_of_children; ++i) {
					auto& seg = child_segments[i];
					if(! seg.empty()) {
						it->nd().children[i].reset( new node(seg) );
						
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



template<typename Traits, typename Point, typename Allocator> template<typename Condition_func>
const Point& tree_point_cloud<Traits, Point, Allocator>::
closest_point(const point_xyz& query, float accepting_distance, float rejecting_distance, const Condition_func& cond) const {
	auto r = root();
	auto it = r.closest_point(query, accepting_distance, rejecting_distance, cond);
	if(it != r.end()) return *it;
	else return super::invalid_point_();
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