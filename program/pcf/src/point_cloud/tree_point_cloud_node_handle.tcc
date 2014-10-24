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
	bt.push(*this);

	if(max_depth == 0) return bt.top();
	
	std::ptrdiff_t i = child_containing_point(p);
	if(has_child(i)) return child(i).deepest_child_containing_point(p, bt, max_depth - 1);
	else return bt.top();
}	




template<typename Traits, typename Point, typename Allocator> template<typename Node>
template<typename Callback_func, typename Order_func>
void tree_point_cloud<Traits, Point, Allocator>::node_handle_template<Node>::ascend
(Callback_func callback, Order_func order, backtrace& bt) const {
	node_handle_template last_child = bt.top();
	bt.pop();
	while(bt.size()) {
		node_handle_template nd = bt.top();
		
		// Get child node handles other than last_child
		std::array<node_handle_template, Traits::number_of_children - 1> other_children;
		std::ptrdiff_t n = 0; // Will after for-loop be number of other_children
		for(std::ptrdiff_t i = 0; i < Traits::number_of_children; ++i) {
			if(nd.has_child(i)) other_children[n] = nd.child(i);
			if(other_children[n] != last_child) ++n;
		}
		
		// Order other_children
		std::array<std::ptrdiff_t, Traits::number_of_children - 1> order; // TODO
		for(std::ptrdiff_t i = 0; i < n; ++i) order[i] = i;
		
		// Callbacks
		for(std::ptrdiff_t i = 0; i < n; ++i) {
			bool can_continue = callback(other_children[order[i]]);
			if(! can_continue) return;
		}
		
		// Ascend further
		bt.pop();
	}	
};


}
