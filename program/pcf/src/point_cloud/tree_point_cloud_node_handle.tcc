#include <type_traits>
#include <stack>

namespace pcf {

template<typename Traits, typename Point, typename Allocator> template<typename Node>
bool tree_point_cloud<Traits, Point, Allocator>::node_handle_template<Node>::has_child(std::ptrdiff_t i) const {
	return bool(nd_->children[i]);
}

template<typename Traits, typename Point, typename Allocator> template<typename Node>
bool tree_point_cloud<Traits, Point, Allocator>::node_handle_template<Node>::is_leaf() const {
	for(const auto& c : nd_->children) if(c) return false;
	return true;
}


template<typename Traits, typename Point, typename Allocator> template<typename Node>
auto tree_point_cloud<Traits, Point, Allocator>::node_handle_template<Node>::child(std::ptrdiff_t i) const -> node_handle_template {
	assert(has_child(i));
	return node_handle_template(
		*(nd_->children[i]),
		Traits::child_cuboid(i, cub_, attr(), depth_),
		depth_ + 1
	);
};


template<typename Traits, typename Point, typename Allocator> template<typename Node>
void tree_point_cloud<Traits, Point, Allocator>::node_handle_template<Node>::make_child(std::ptrdiff_t i, const segment& seg) const {
	nd_->children[i].reset( new node(seg) );
}


template<typename Traits, typename Point, typename Allocator> template<typename Node>
template<typename Other_point>
std::ptrdiff_t tree_point_cloud<Traits, Point, Allocator>::node_handle_template<Node>::child_containing_point(const Other_point& p) const {
	return Traits::child_containing_point(p, cub_, attr(), depth_);
};


template<typename Traits, typename Point, typename Allocator> template<typename Node>
template<typename Other_point>
auto tree_point_cloud<Traits, Point, Allocator>::node_handle_template<Node>::deepest_child_containing_point
(const Other_point& p, std::ptrdiff_t max_depth) const -> node_handle_template {
	if(max_depth == 0) return *this;
	
	std::ptrdiff_t i = child_containing_point(p);
	if(has_child(i)) return child(i).deepest_child_containing_point(p, max_depth - 1);
	else return *this;
}



template<typename Traits, typename Point, typename Allocator> template<typename Node>
template<typename Other_point>
auto tree_point_cloud<Traits, Point, Allocator>::node_handle_template<Node>::deepest_child_containing_point
(const Other_point& p, backtrace& bt, std::ptrdiff_t max_depth) const -> node_handle_template& {
	bt.push_back(*this);

	if(max_depth == 0) return bt.back();
	
	std::ptrdiff_t i = child_containing_point(p);
	if(has_child(i)) return child(i).deepest_child_containing_point(p, bt, max_depth - 1);
	else return bt.back();
}	



template<typename Traits, typename Point, typename Allocator> template<typename Node>
template<typename Callback_func, typename Order_func>
bool tree_point_cloud<Traits, Point, Allocator>::node_handle_template<Node>::depth_first_descent
(Callback_func callback, Order_func order) const {
	bool can_continue = callback(*this);
	if(! can_continue) return false;
	
	std::array<node_handle_template, Traits::number_of_children - 1> children;
	std::ptrdiff_t n = 0;
	for(std::ptrdiff_t i = 0; i < Traits::number_of_children; ++i) {
		if(has_child(i)) children[n++] = child(i);
	}
	
	std::sort(children.begin(), children.begin()+n, order);
	
	for(std::ptrdiff_t i = 0; i < n; ++i) {
		can_continue = children[i].depth_first_descent(callback, order);
		if(! can_continue) return false;
	}
	
	return true;
}


template<typename Traits, typename Point, typename Allocator> template<typename Node>
template<typename Callback_func, typename Order_func>
void tree_point_cloud<Traits, Point, Allocator>::node_handle_template<Node>::visit_neightboring_nodes
(Callback_func callback, Order_func order, const backtrace& bt) const {
	auto last_it = bt.rbegin();
	callback(*last_it);
	
	if(bt.size() == 1) return;
	
	for(auto it = bt.rbegin() + 1; it != bt.rend(); ++it) {				
		// Get child node handles other than last_child
		std::array<node_handle_template, Traits::number_of_children - 1> other_children;
		std::ptrdiff_t n = 0; // Will after for-loop be number of other_children
		for(std::ptrdiff_t i = 0; i < Traits::number_of_children; ++i) {
			if(it->has_child(i)) {
				other_children[n] = it->child(i);
				if(other_children[n] != *last_it) ++n;	
			}
		}
		
		// Order other_children
		std::sort(other_children.begin(), other_children.begin()+n, order);
		
		// Callbacks
		for(std::ptrdiff_t i = 0; i < n; ++i) {
			bool can_continue = callback(other_children[i]);
			if(! can_continue) return;
		}
		
		// Ascend further
		last_it = it;
	}
}


template<typename Traits, typename Point, typename Allocator> template<typename Node>
template<typename Callback_func, typename Order_func>
void tree_point_cloud<Traits, Point, Allocator>::node_handle_template<Node>::visit_neightboring_leaves
(Callback_func callback, Order_func order, const backtrace& bt) const {
	auto leaf_callback = [&callback](const node_handle_template& l) {
		if(l.is_leaf()) return callback(l);
		else return true;
	};

	visit_neightboring_nodes([&order, &leaf_callback](const node_handle_template& n) {
		return n.depth_first_descent(leaf_callback, order);
	}, order, bt);
}



template<typename Traits, typename Point, typename Allocator> template<typename Node>
template<typename Inserter>
void tree_point_cloud<Traits, Point, Allocator>::node_handle_template<Node>::locality_(std::size_t k, const backtrace& bt, Inserter ins) const {
	const auto& query_node = bt.back();

	float prune_dist = 0;	
	int remaining = k;
	query_node.visit_neightboring_leaves(
		[&query_node, &ins, &remaining, &prune_dist](const node_handle_template& nd) {
			remaining -= nd.size();
			*ins = nd;
			prune_dist = cuboid::maximal_distance_sq(query_node.cub(), nd.cub());
			return (remaining >= 0);
		},
		[&query_node](const node_handle_template& a, const node_handle_template& b) {
			float max_dist_a = cuboid::maximal_distance_sq(query_node.cub(), a.cub());
			float max_dist_b = cuboid::maximal_distance_sq(query_node.cub(), b.cub());
			return (max_dist_a < max_dist_b);
		},
		bt
	);
	
	query_node.visit_neightboring_leaves(
		[&query_node, &ins, prune_dist](const node_handle_template& nd) {
			float min_dist = cuboid::minimal_distance_sq(query_node.cub(), nd.cub());
			*ins = nd;
			return (min_dist <= prune_dist);
		},
		[&query_node](const node_handle_template& a, const node_handle_template& b) {
			float min_dist_a = cuboid::minimal_distance_sq(query_node.cub(), a.cub());
			float min_dist_b = cuboid::minimal_distance_sq(query_node.cub(), b.cub());
			return (min_dist_a < min_dist_b);
		},
		bt
	);
}



}
