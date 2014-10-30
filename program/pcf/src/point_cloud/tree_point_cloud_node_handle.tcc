#include <type_traits>
#include <stack>
#include <algorithm>
#include <iostream>

namespace pcf {

template<typename Traits, typename Point, typename Allocator> template<typename Node>
bool tree_point_cloud<Traits, Point, Allocator>::node_handle_<Node>::has_child(std::ptrdiff_t i) const {
	return bool(nd_->children[i]);
}

template<typename Traits, typename Point, typename Allocator> template<typename Node>
bool tree_point_cloud<Traits, Point, Allocator>::node_handle_<Node>::is_leaf() const {
	for(const auto& c : nd_->children) if(c) return false;
	return true;
}


template<typename Traits, typename Point, typename Allocator> template<typename Node>
auto tree_point_cloud<Traits, Point, Allocator>::node_handle_<Node>::child(std::ptrdiff_t i) const -> node_handle_ {
	assert(has_child(i));
	return node_handle_(
		*(nd_->children[i]),
		Traits::child_box(i, box_, attr(), depth_),
		depth_ + 1
	);
};


template<typename Traits, typename Point, typename Allocator> template<typename Node>
void tree_point_cloud<Traits, Point, Allocator>::node_handle_<Node>::make_child(std::ptrdiff_t i, const segment& seg) const {
	nd_->children[i].reset( new node(seg) );
}


template<typename Traits, typename Point, typename Allocator> template<typename Node>
template<typename Other_point>
std::ptrdiff_t tree_point_cloud<Traits, Point, Allocator>::node_handle_<Node>::child_containing_point(const Other_point& p) const {
	return Traits::child_containing_point(p, box_, attr(), depth_);
};


template<typename Traits, typename Point, typename Allocator> template<typename Node>
template<typename Other_point>
auto tree_point_cloud<Traits, Point, Allocator>::node_handle_<Node>::deepest_child_containing_point
(const Other_point& p, std::ptrdiff_t max_depth) const -> node_handle_ {
	if(max_depth == 0) return *this;
	
	std::ptrdiff_t i = child_containing_point(p);
	if(has_child(i)) return child(i).deepest_child_containing_point(p, max_depth - 1);
	else return *this;
}



template<typename Traits, typename Point, typename Allocator> template<typename Node>
template<typename Other_point>
auto tree_point_cloud<Traits, Point, Allocator>::node_handle_<Node>::deepest_child_containing_point
(const Other_point& p, backtrace& bt, std::ptrdiff_t max_depth) const -> node_handle_& {
	bt.push_back(*this);

	if(max_depth == 0) return bt.back();
	
	std::ptrdiff_t i = child_containing_point(p);
	if(has_child(i)) return child(i).deepest_child_containing_point(p, bt, max_depth - 1);
	else return bt.back();
}	



template<typename Traits, typename Point, typename Allocator> template<typename Node>
template<typename Callback_func, typename Order_func>
bool tree_point_cloud<Traits, Point, Allocator>::node_handle_<Node>::depth_first_descent
(Callback_func callback, Order_func order) const {
	bool can_continue = callback(*this);
	if(! can_continue) return false;
	
	std::array<node_handle_, Traits::number_of_children - 1> children;
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


}
