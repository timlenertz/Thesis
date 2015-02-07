#include <utility>
#include <cstring>
#include <stdexcept>

namespace pcf {

template<typename Point, typename Allocator> template<typename Other_point, typename Other_allocator>
unorganized_point_cloud_stack<Point, Allocator>::unorganized_point_cloud_stack(const point_cloud<Other_point, Other_allocator>& pc, std::size_t capacity, bool all_val, const Allocator& alloc) :
	super(pc, capacity, all_val, alloc) { }


template<typename Point, typename Allocator>
unorganized_point_cloud_stack<Point, Allocator>::unorganized_point_cloud_stack(point_cloud<Point, Allocator>&& pc, bool all_val) :
	super(std::move(pc), all_val) { }


template<typename Point, typename Allocator>
unorganized_point_cloud_stack<Point, Allocator>::~unorganized_point_cloud_stack() {
	clear_stack();
}



template<typename Point, typename Allocator>
void unorganized_point_cloud_stack<Point, Allocator>::push() {
	push(super::capacity());
}


template<typename Point, typename Allocator>
void unorganized_point_cloud_stack<Point, Allocator>::push(std::size_t new_capacity) {
	if(new_capacity < super::size())
		throw std::logic_error("New capacity cannot be smalled than current point cloud size.");

	// Stack current
	stacked s;
	s.begin = super::begin_;
	s.end = super::end_;
	s.allocated_size = super::allocated_size_;
	stack_.push(s);
	
	// Allocate new
	std::size_t sz = super::size();
	super::begin_ = super::allocator_.allocate(new_capacity);
	super::end_ = super::begin_ + sz;
	super::allocated_size_ = new_capacity;
	
	// Copy points
	std::memcpy(
		static_cast<void*>(super::begin_),
		static_cast<const void*>(s.begin),
		sz * sizeof(Point)
	);
}

template<typename Point, typename Allocator>
void unorganized_point_cloud_stack<Point, Allocator>::pop() {
	if(stack_.empty())
		throw std::logic_error("Point cloud stack is empty.");
	
	// Deallocate current
	super::allocator_.deallocate(super::begin_, super::allocated_size_);
	
	// Restore from stack
	stacked& s = stack_.top();
	super::begin_ = s.begin;
	super::end_ = s.end;
	super::allocated_size_ = s.allocated_size;
	stack_.pop();
}


template<typename Point, typename Allocator>
std::size_t unorganized_point_cloud_stack<Point, Allocator>::stack_size() const {
	return stack_.size();
}


template<typename Point, typename Allocator>
void unorganized_point_cloud_stack<Point, Allocator>::clear_stack() {
	while(! stack_.empty()) {
		stacked& s = stack_.top();
		super::allocator_.deallocate(s.begin, s.allocated_size);
		stack_.pop();
	}
}


}