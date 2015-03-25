#include <cmath>
#include <set>
#include <iostream>

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
plane fit_plane_to_points(Iterator begin, Iterator end) {
	std::size_t n = end - begin;
	Eigen::Vector3f center = center_of_mass(begin, end);

	Eigen::MatrixX3f mat(n, 3);
	std::ptrdiff_t i = 0;
	for(Iterator it = begin; it != end; ++it, ++i)
		mat.row(i) = ( (Eigen::Vector3f)*it ).transpose();

	Eigen::JacobiSVD<Eigen::MatrixX3f> svd(mat, Eigen::ComputeThinV | Eigen::ComputeThinU);
	Eigen::Vector3f norm = svd.solve(-Eigen::Vector3f::Ones());
		
	return plane(center, norm);
}


}