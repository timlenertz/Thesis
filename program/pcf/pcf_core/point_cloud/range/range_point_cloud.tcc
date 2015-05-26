#include <cmath>
#include <cassert>
#include <utility>
#include <cmath>
#include <algorithm>
#include <iostream>
#include "../../image/image.h"
#include "../../image/range_image.h"
#include "../../image/rgb_color_image.h"
#include "../../io/range_point_cloud_importer.h"
#include "../../point.h"

namespace pcf {

template<typename Point, typename Allocator>
multi_dimensional_buffer<Point, 2> range_point_cloud<Point, Allocator>::create_image_(bool row_major, std::size_t w, std::size_t h, Point* buf) {
	size_2dim sz;
	if(row_major) sz = size_2dim(h, w);
	else sz = size_2dim(w, h);
	std::size_t len = w * h;
	return multi_dimensional_buffer<Point, 2>(sz, buf, buf + len);
}

template<typename Point, typename Allocator>
range_point_cloud<Point, Allocator>::range_point_cloud(std::size_t w, std::size_t h, bool row_major, const Allocator& alloc) :
	super(w * h, alloc),
	image_(create_image_(row_major, w, h, super::begin_)),
	row_major_order_(row_major)
{
	super::resize_(super::capacity());
}



template<typename Point, typename Allocator>
range_point_cloud<Point, Allocator>::range_point_cloud(range_point_cloud_importer& imp, const Allocator& alloc) :
	range_point_cloud(imp.columns(), imp.rows(), true, alloc)
{
	if(imp.has_camera_pose())
		super::set_relative_pose( imp.camera_pose() );

	super::resize_(super::capacity());
	for(Point* buf = super::begin_; buf < super::end_; buf += imp.columns())
		imp.read_row(buf);
}


template<typename Point, typename Allocator> template<typename Other_point, typename Other_allocate>
range_point_cloud<Point, Allocator>::range_point_cloud(const range_point_cloud<Other_point, Other_allocate>& pc, const Allocator& alloc) :
	super(pc, 0, alloc),
	image_(create_image_(pc.row_major_order(), pc.width(), pc.height(), super::begin_)),
	row_major_order_(pc.row_major_order()) { }


template<typename Point, typename Allocator>
range_point_cloud<Point, Allocator>::range_point_cloud(range_point_cloud&& pc) :
	super(std::move(pc)),
	image_(pc.image_),
	row_major_order_(pc.row_major_order()) { }



template<typename Point, typename Allocator>
std::size_t range_point_cloud<Point, Allocator>::width() const {
	return image_.size()[row_major_order_ ? 1 : 0];
}


template<typename Point, typename Allocator>
std::size_t range_point_cloud<Point, Allocator>::height() const {
	return image_.size()[row_major_order_ ? 0 : 1];
}


template<typename Point, typename Allocator>
const Point& range_point_cloud<Point, Allocator>::at(std::ptrdiff_t x, std::ptrdiff_t y) const {
	if(row_major_order_) return image_[{ y, x }];
	else return image_[{ x, y }];
}



template<typename Point, typename Allocator>
Point& range_point_cloud<Point, Allocator>::at(std::ptrdiff_t x, std::ptrdiff_t y) {
	if(row_major_order_) return image_[{ y, x }];
	else return image_[{ x, y }];
}



template<typename Point, typename Allocator>
range_image range_point_cloud<Point, Allocator>::to_range_image() const {
	range_image ri(width(), height());
	for(auto it = image_.begin(); it != image_.end(); ++it) {
		auto ind = it.index();
		if(it->valid()) ri[ind] = it->coordinates().norm();
		else ri.invalidate(ind);
	}
	return ri;
}


template<typename Point, typename Allocator>
masked_intensity_image range_point_cloud<Point, Allocator>::weights_to_intensity_image() const {
	masked_intensity_image ii(width(), height());
	for(auto it = image_.begin(); it != image_.end(); ++it) {
		auto ind = it.index();
		if(it->valid()) ii[ind] = it->get_weight();
		else ii.invalidate(ind);
	}
	return ii;
}



template<typename Point, typename Allocator>
rgb_color_image range_point_cloud<Point, Allocator>::to_rgb_color_image(rgb_color bg) const {
	rgb_color_image ci(width(), height());
	for(auto it = image_.begin(); it != image_.end(); ++it) {
		auto ind = it.index();
		rgb_color& col = ci[ind];
		if(it->valid()) col = it->get_color();
		else col = bg;
	}
	return ci;
}



template<typename Point, typename Allocator> template<typename That, typename Condition_func, typename Callback_func>
void range_point_cloud<Point, Allocator>::nearest_neighbors_
(That that, std::size_t k, Condition_func cond, Callback_func callback, bool par) {
	const std::size_t maximal_expansions = 100;
	
	using point_ptr_t = decltype(that->data());
	using iterator_t = dereference_iterator<typename std::vector<point_ptr_t>::const_iterator>;

	//#pragma omp parallel for if(par)
	for(auto it = that->image_.begin(); it != that->image_.end(); ++it) {
		auto& ref = *it;
		if(! it->valid()) continue;
		if(! cond(ref)) continue;
		
		std::vector<point_ptr_t> knn;
		knn.reserve(k * 2);
		if(ref.valid()) knn.push_back(&ref);
	
		auto process = [&](point_ptr_t p) {
			if(p->valid()) knn.push_back(p);
		};
		auto cmp = [&](point_ptr_t a, point_ptr_t b) {
			float da = distance_sq(ref, *a);
			float db = distance_sq(ref, *b);
			return (da < db);
		};
	
	
		image_coordinates ref_ic = it.index();
		image_coordinates o = ref_ic, e = ref_ic;
		image_coordinates ic = ref_ic;
		std::size_t expansions = 0;
		while(knn.size() < k && ++expansions < maximal_expansions) {
			if(o.y > 0) {
				ic.y = --o.y;
				for(ic.x = o.x; ic.x <= e.x; ++ic.x) process(&that->image_[ic]);
			}
			if(e.y < that->height() - 1) {
				ic.y = ++e.y;
				for(ic.x = o.x; ic.x <= e.x; ++ic.x) process(&that->image_[ic]);
			}
			if(o.x > 0) {
				ic.x = --o.x;
				for(ic.y = o.y; ic.y <= e.y; ++ic.y) process(&that->image_[ic]);
			}
			if(e.x < that->width() - 1) {
				ic.x = ++e.x;
				for(ic.y = o.y; ic.y <= e.y; ++ic.y) process(&that->image_[ic]);
			}		
		}
		
		std::nth_element(knn.begin(), knn.begin()+(k-1), knn.end(), cmp);
		iterator_t begin(knn.begin());
		iterator_t end(knn.begin() + k);

		callback(ref, begin, end);
	}
}


template<typename Point, typename Allocator>
float range_point_cloud<Point, Allocator>::ratio_of_area_filled() const {
	float total = width() * height();
	float filled = super::number_of_valid_points();
	return filled / total;
}


}