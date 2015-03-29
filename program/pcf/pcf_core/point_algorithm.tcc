#include <cmath>
#include <set>
#include <iostream>
#include "field/field.h"

namespace pcf {

template<typename Point, typename Iterator>
Iterator find_closest_point(const Point& ref, Iterator begin, Iterator end, float accepting_distance) {
	float min_distance = INFINITY;
	Iterator closest_point = end;
	
	for(Iterator it = begin; it != end; ++it) {
		float d = distance_sq(*it, ref);
		if(d < min_distance) {
			min_distance = d;
			closest_point = it;
			if(d <= accepting_distance) break;
		}
	}
	
	return closest_point;
}


template<typename Point, typename Iterator>
point_cloud_selection<Point> find_nearest_neighbors(const Point& ref, Iterator begin, Iterator end, std::size_t k) {
	auto cmp = [&](Point* a, Point* b) {
		float da = distance_sq(*a, ref);
		float db = distance_sq(*b, ref);
		return (da < db);
	};
	std::set<Point*, decltype(cmp)> closest(cmp);
	
	for(Iterator it = begin; it != end; ++it) {
		Point& pt = *it;
		if(closest.size() < k) {
			closest.insert(&pt);
		} else {
			float d = distance_sq(*it, ref);
			Point* furthest_of_the_closest = *closest.rbegin();
			if(d < distance_sq(*furthest_of_the_closest, ref)) {
				closest.erase(furthest_of_the_closest);
				closest.insert(&pt);
			}
		}
	}
	
	point_cloud_selection<Point> sel;
	for(Point* pt : closest)
		sel.add(*pt);
	return sel;
}


template<typename Iterator>
void set_unique_color(Iterator begin, Iterator end, rgb_color col) {
	for(Iterator it = begin; it != end; ++it) it->set_color(col);
}


template<typename Iterator>
void colorize_by_weight(Iterator begin, Iterator end, float min_w, float max_w, const rgb_color& col_min, const rgb_color& col_max) {
	for(Iterator it = begin; it != end; ++it) {
		if(! it->valid()) continue;
		float w = it->get_weight();
		rgb_color col;
		if(w < min_w) {
			col = col_min;
		} else if(w > max_w) {
			col = col_max;
		} else {
			float diff = max_w - min_w;
			float rw = (w - min_w) / diff;
			col = (1.0 - rw)*col_min + rw*col_max;
		}
		it->set_color(col);
	}
}


template<typename Iterator>
void set_unique_weight(Iterator begin, Iterator end, std::uint8_t w) {
	for(Iterator it = begin; it != end; ++it) it->set_weight(w);
}


template<typename Iterator>
void normalize_point_weights(Iterator begin, Iterator end) {
	float sum_of_weights = 0.0;
	std::size_t number_of_points = 0;
	for(Iterator it = begin; it != end; ++it) {
		sum_of_weights += it->get_weight();
		++number_of_points;
	}
	float factor = (float)number_of_points / sum_of_weights;
	for(Iterator it = begin; it != end; ++it) {
		float w = it->get_weight();
		it->set_weight(w * factor);
	}
}


template<typename Iterator>
Eigen::Vector3f center_of_mass(Iterator begin, Iterator end) {
	Eigen::Vector3f sum = Eigen::Vector3f::Zero();
	std::size_t n = 0;
	for(Iterator it = begin; it != end; ++it) {
		sum += (Eigen::Vector3f)*it;
		++n;
	}
	return sum / n;
}

template<typename Iterator>
Eigen::Vector3f weighted_center_of_mass(Iterator begin, Iterator end) {
	Eigen::Vector3f weighted_sum = Eigen::Vector3f::Zero();
	float total_weight = 0.0;
	for(Iterator it = begin; it != end; ++it) {
		float w = it->get_weight();
		weighted_sum += w * (Eigen::Vector3f)*it;
		total_weight += w;
	}
	return weighted_sum / total_weight;
}


template<typename Iterator>
void orient_normals_to_point(Iterator begin, Iterator end, const Eigen::Vector3f& o, bool away) {
	for(Iterator it = begin; it != end; ++it) {
		auto& p = *it;
		if(! p.valid()) continue;
		
		Eigen::Vector3f po = o - p.coordinates();
		Eigen::Vector3f n = p.get_normal(false);
		float dot = po.dot(n);
		if( (dot < 0.0) != away ) p.flip_normal();
	}
}

template<typename Iterator>
plane fit_plane_to_points(Iterator begin, Iterator end) {
	std::size_t n = end - begin;
	Eigen::Vector3f center = center_of_mass(begin, end);

	Eigen::MatrixXf mat(n, 4);
	std::ptrdiff_t i = 0;
	for(Iterator it = begin; it != end; ++it, ++i) {
		const auto& pt = *it;
		mat(i, 0) = pt[0];
		mat(i, 1) = pt[1];
		mat(i, 2) = pt[2];
		mat(i, 3) = 1.0;
	}

	Eigen::JacobiSVD<Eigen::MatrixX3f> svd(mat, Eigen::ComputeThinV);
	auto v = svd.matrixV();
	Eigen::Vector3f norm(v(0, 0), v(0, 1), v(0, 2));
		
	return plane(center, norm);
}


template<typename Iterator>
bounding_box compute_bounding_box(Iterator begin, Iterator end, float ep) {
	const float inf = INFINITY;
	Eigen::Vector4f mn(+inf, +inf, +inf, 0);
	Eigen::Vector4f mx(-inf, -inf, -inf, 0);

	#pragma omp parallel
	{
		Eigen::Vector4f mn_part(+inf, +inf, +inf, 0);
		Eigen::Vector4f mx_part(-inf, -inf, -inf, 0);
		
		#pragma omp for
		for(Iterator p = begin; p < end; ++p) {
			if(! p->valid()) continue;
			const Eigen::Vector4f pc = p->homogeneous_coordinates;
			
			mn_part = mn_part.cwiseMin(pc);
			mx_part = mx_part.cwiseMax(pc);
		}
		
		#pragma omp critical
		{
			mn = mn.cwiseMin(mn_part);
			mx = mx.cwiseMax(mx_part);
		}
	}
		
	mx += Eigen::Vector4f(ep, ep, ep, 0);
	
	return bounding_box(mn.head(3), mx.head(3));
}

}