#ifndef PCF_POINT_CLOUD_SEGMENT_UNION_H_
#define PCF_POINT_CLOUD_SEGMENT_UNION_H_

#include <iterator>
#include <vector>
#include <initializer_list>
#include <type_traits>

#include "segment.h"

namespace pcf {

/**
Union of point cloud segments.
Provides iterator that goes through the points in one of the segments, in the global (point cloud) order.
*/
template<typename Point>
class point_cloud_segment_union {
public:
	using segment_type = point_cloud_segment<Point>;

private:
	using segments_set = std::vector<segment_type>;
	segments_set segments_;
	
	void initialize_();
	
public:
	class iterator;
	class const_iterator;

	point_cloud_segment_union(std::initializer_list<segment_type>);
	explicit point_cloud_segment_union(const std::vector<segment_type>&);
	explicit point_cloud_segment_union(std::vector<segment_type>&&);
		
	std::size_t number_of_segments() const { return segments_.size(); }
	
	std::size_t size() const;
	
	iterator begin();
	const_iterator cbegin() const;
	const_iterator begin() const { return cbegin(); }
	
	iterator end();
	const_iterator cend() const;
	const_iterator end() const { return cend(); }
};

}

#include "segment_union.tcc"

#endif