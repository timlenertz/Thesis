#include <cmath>
#include <set>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <cassert>
#include "field/field.h"
#include "util/random.h"
#include "util/misc.h"

namespace pcf {

template<typename Point, typename Iterator>
Iterator find_closest_point(const Point& ref, Iterator begin, Iterator end, float accepting_distance) {
	return find_closest_point(ref, begin, end, accepting_distance, [](const point_xyz&){ return true; });
}


template<typename Point, typename Iterator, typename Condition_func>
Iterator find_closest_point(const Point& ref, Iterator begin, Iterator end, float accepting_distance, const Condition_func& cond) {
	float accepting_distance_sq = accepting_distance*accepting_distance;
	
	float min_distance_sq = INFINITY;
	Iterator closest_point = end;
	
	for(Iterator it = begin; it != end; ++it) {
		float d = distance_sq(*it, ref);
		if(d < min_distance_sq && cond(*it)) {
			min_distance_sq = d;
			closest_point = it;
			if(d <= accepting_distance_sq) break;
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
void compute_normal_direction_weights(Iterator begin, Iterator end, const Eigen::Vector3f& ref, angle maximal_angle) {
	float min_cos = std::cos(maximal_angle);

	for(Iterator it = begin; it != end; ++it) {
		auto& p = *it;
		if(! p.valid()) continue;
		
		Eigen::Vector3f n = p.get_normal(true);
		Eigen::Vector3f pr = (ref - p.coordinates()).normalized();
		float cos_angle = n.dot(pr);
		
		float w = 0.0;		
		if(cos_angle > min_cos) w = (cos_angle - min_cos) / (1.0 - min_cos);
		else w = 0.0;
		
		it->set_weight(w);
	}
}

template<typename Iterator>
plane fit_plane_to_points(Iterator begin, Iterator end) {
	Eigen::Matrix3f A = Eigen::Matrix3f::Zero();
	Eigen::Vector3f b = Eigen::Vector3f::Zero();
	for(Iterator it = begin; it != end; ++it) {
		const auto& p = *it;
		if(! p.valid()) continue;
		
		Eigen::Matrix3f A_term;
		A_term <<
			p[0]*p[0], p[0]*p[1], p[0],
			p[0]*p[1], p[1]*p[1], p[1],
			p[0], p[1], 1.0;
		Eigen::Vector3f b_term( p[0]*p[2], p[1]*p[2], p[2] );

		A += A_term;
		b += b_term;
	}
	
	Eigen::Vector3f x = A.inverse() * b;
	return plane(x[0], x[1], -1.0, x[2]);
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



template<typename Iterator>
void compute_lambertian_illumination_weights(Iterator begin, Iterator end, const Eigen::Vector3f& light_source) {
	for(Iterator it = begin; it != end; ++it) {
		auto& p = *it;
		if(! p.valid()) continue;
		
		Eigen::Vector3f n = p.get_normal(true);
		Eigen::Vector3f l = (p.coordinates() - light_source).normalized();
		
		float w = l.dot(n);
		if(w < 0) w = 0.0;
		it->set_weight(w);
	}
}


template<typename Iterator>
plane compute_tangent_plane(const point_xyz& p0, Iterator neighbors_begin, Iterator neighbors_end) {
	const int max_iterations = 10;

	plane best_pl;
	float old_avg_distance = INFINITY;

	std::size_t k = std::distance(neighbors_begin, neighbors_end);
	assert(k >= 2);

	std::vector<float> distances;
	distances.reserve(k);
	
	float old_a = 0;
	
	for(int iteration = 0; iteration < max_iterations; ++iteration) {
		distances.clear();
	
		// Choose 2 random points
		std::ptrdiff_t i1 = random_integer<std::ptrdiff_t>(0, k - 1);
		std::ptrdiff_t i2 = random_integer<std::ptrdiff_t>(0, k - 2);
		if(i2 >= i1) ++i2;
	
		const point_xyz& p1 = *(neighbors_begin + i1);
		const point_xyz& p2 = *(neighbors_begin + i2);
		
		Eigen::Vector3f u = p1.coordinates() - p0.coordinates(), v = p1.coordinates() - p0.coordinates();
		float a = sq(u[1]*v[2] - u[2]*v[1]) + sq(u[2]*v[0] - u[0]*v[2]) + sq(u[0]*v[1] - u[1]*v[0]);
		if(a < 0.9*old_a) continue;
		else old_a = a;

		// Fit plane through 3 points
		plane pl(p0, p1, p2);
		
		// Get average and max distance of points to plane
		float avg_distance = 0, max_distance = 0;
		for(Iterator it = neighbors_begin; it != neighbors_end; ++it) {
			float d = distance(*it, pl);
			distances.push_back(d);
			avg_distance += d;
			if(d > max_distance) max_distance = d;
		}
		avg_distance /= (k - 1);
		
		// Set threshold distance for consensus set
		float threshold_distance;
		if(max_distance > 2.0*avg_distance) threshold_distance = 2.0*avg_distance;
		else threshold_distance = max_distance;
		
		// Count points in consensus set
		std::size_t c = 0;
		avg_distance = 0;
		for(float d : distances) if(d <= threshold_distance) {
			++c;
			avg_distance += d;
		}
		avg_distance /= c;
		
		if(avg_distance >= old_avg_distance) continue;
		
		old_avg_distance = avg_distance;
		best_pl = pl;
	}

	return best_pl;
}

template<typename Iterator>
float compute_local_surface_density(const point_full& q, Iterator neighbors_begin, Iterator neighbors_end) {
	Eigen::Vector3f nq = q.get_normal();
	plane pl(q, nq);

	float max_radius = 0;
	std::size_t count = 0;
	for(Iterator it = neighbors_begin; it != neighbors_end; ++it) {
		const point_full& p = *it;
		//Eigen::Vector3f np = p.get_normal();
		//float dot = nq.dot(np);
		
		//static const float max_angle_cos = std::cos(angle::degrees(25.0));
		//if(dot >= max_angle_cos) {
			Eigen::Vector3f pp = pl.project(p.coordinates());
			float r = (pp - pl.project(q)).norm();
			if(r > max_radius) max_radius = r;
			++count;
		//}
	}
	if(count == 0) return NAN;
	
	return count / (pi * max_radius * max_radius);	
}


template<typename Iterator>
float compute_local_surface_curvature(const point_full& p, Iterator neighbors_begin, Iterator neighbors_end, float A, float D) {
	std::vector<point_full> neighbors(neighbors_begin, neighbors_end);
	point_full* closest;
	float closest_d_sq = INFINITY;
	for(auto it = neighbors_begin; it != neighbors_end; ++it) {
		float d_sq = distance_sq(*it, p);
		if(d_sq == 0.0) continue;
		else if(d_sq < closest_d_sq) {
			closest_d_sq = d_sq;
			closest = &(*it);
		}
	}	
	
	float weight_sum = 0;
	float sum = 0;
	for(auto it = neighbors_begin; it != neighbors_end; ++it) {
		point_full& q = *it;
		if(! q.valid()) continue;
		
		float d_sq = distance_sq(q, p);
		
		float w;	
		if(d_sq == 0.0) continue;
		else if(&q == closest) w = 1.0;
		else w = std::sqrt(closest_d_sq / d_sq);
		weight_sum += w;
		
		float a = std::abs(half_pi * (1.0 - p.get_normal().dot(q.get_normal())));
		float d = std::abs(p.get_normal().dot(p.coordinates() - q.coordinates()));
				
		sum += w * (A*a + D*d);
	}
	
	float c = sum / (weight_sum * (neighbors.size() - 1));

	return c;
}


}