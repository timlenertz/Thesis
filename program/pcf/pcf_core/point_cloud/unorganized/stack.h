#ifndef PCF_UNORGANIZED_POINT_CLOUD_STACK_H_
#define PCF_UNORGANIZED_POINT_CLOUD_STACK_H_

#include "unorganized_point_cloud.h"
#include <stack>

namespace pcf {

template<typename Point, typename Allocator = default_allocator<Point>>
class unorganized_point_cloud_stack : public unorganized_point_cloud<Point, Allocator> {
	using super = unorganized_point_cloud<Point, Allocator>;

private:
	struct stacked {
		Point* begin;
		Point* end;
		std::size_t allocated_size;
	};

	std::stack<stacked> stack_;
	
public:
	template<typename Other_point, typename Other_allocator>
	unorganized_point_cloud_stack(const point_cloud<Other_point, Other_allocator>&, std::size_t capacity = 0, bool all_val = true, const Allocator& = Allocator());

	unorganized_point_cloud_stack(point_cloud<Point, Allocator>&&, bool all_val = true);

	~unorganized_point_cloud_stack();
	
	void push();
	void push(std::size_t new_capacity);
	void pop();
	
	std::size_t stack_size() const;
	void clear_stack();
};

extern template class unorganized_point_cloud_stack<point_xyz>;
extern template class unorganized_point_cloud_stack<point_full>;

using unorganized_point_cloud_stack_xyz = unorganized_point_cloud_stack<point_xyz>;
using unorganized_point_cloud_stack_full = unorganized_point_cloud_stack<point_full>;

}

#include "stack.tcc"

#endif