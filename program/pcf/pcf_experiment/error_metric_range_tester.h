#ifndef PCFEX_ERROR_METRIC_RANGE_TESTER_H_
#define PCFEX_ERROR_METRIC_RANGE_TESTER_H_

#include "../pcf_core/geometry/pose.h"
#include "error_metric_tester.h"
#include <ostream>
#include <iostream>
#include <fstream>

namespace pcf { namespace exper {

template<typename Error_metric, typename Correspondences>
class error_metric_range_tester : public error_metric_tester<Error_metric, Correspondences> {
	using super = error_metric_tester<Error_metric, Correspondences>;

public:	
	error_metric_range_tester(const Correspondences& cor, const Error_metric& err) :
		super(cor, err) { }


	Eigen::Affine3f intermediate_transformation(const Eigen::Affine3f& minimum_transformation, const Eigen::Affine3f& maximum_transformation, float t) const {
		Eigen::Vector3f min_translation = minimum_transformation.translation();
		Eigen::Vector3f max_translation = maximum_transformation.translation();
		
		Eigen::Quaternionf min_rotation(minimum_transformation.rotation());
		Eigen::Quaternionf max_rotation(maximum_transformation.rotation());
		
		Eigen::Vector3f translation = min_translation + t*(max_translation - min_translation);
		Eigen::Quaternionf rotation(min_rotation.slerp(t, max_rotation));
		
		Eigen::Affine3f result = Eigen::Translation3f(translation) * rotation;
		return result;
	}
	
	void test_range_1dim(std::ostream& str, const Eigen::Affine3f& max_t, int steps = 100) const {
		str << "t e" << std::endl;
		for(int i = -steps; i <= steps; ++i) {
			float t = float(i) / steps;

			Eigen::Affine3f T;
			Eigen::Affine3f I = Eigen::Affine3f::Identity();
			if(i < 0) T = intermediate_transformation(I, max_t.inverse(), -t);
			else if(i == 0) T = Eigen::Affine3f::Identity();
			else if(i > 0) T = intermediate_transformation(I, max_t, t);
			
			float err = (*this)(T);
			str << t << " " << err << std::endl;
		}
	}
	
	void test_range_2dim(std::ostream& str, const Eigen::Affine3f& max_tx, const Eigen::Affine3f& max_ty, int steps = 100) const {
		Eigen::Affine3f I = Eigen::Affine3f::Identity();
		
		for(int y = -steps; y <= steps; ++y) {
			float ty = float(ty)/steps;
			Eigen::Affine3f Ty;
			if(y < 0) Ty = intermediate_transformation(I, max_ty.inverse(), -ty);
			else if(y == 0) Ty = Eigen::Affine3f::Identity();
			else if(y > 0) Ty = intermediate_transformation(I, max_ty, +ty);

			for(int x = -steps; x <= steps; ++x) {
				float tx = float(x)/steps;
				Eigen::Affine3f Tx;
				if(x < 0) Tx = intermediate_transformation(I, max_tx.inverse(), -tx);
				else if(x == 0) Tx = Eigen::Affine3f::Identity();
				else if(x > 0) Tx = intermediate_transformation(I, max_tx, +tx);

				Eigen::Affine3f T = Tx * Ty;
				float err = (*this)(T);
				str << ty << tx << " " << err << std::endl;
			}
		}
	}
	
	void test_range_1dim(const std::string& fname, const Eigen::Affine3f& max_t, int steps = 100) const {
		std::ofstream str(fname);
		test_range_1dim(str, max_t, steps);
	}
	
	void test_random_ranges_1dim(std::ostream& str, float translation_mag = 1.0, angle rotation_mag = angle::degrees(10.0), int n = 10, int steps = 100) {
		str << "t";
		for(int j = 0; j < n; ++j) str << " e" << (j+1);
		str << std::endl;

		std::vector<Eigen::Affine3f> extrema(n);
		
		for(int j = 0; j < n; ++j) {
			extrema[j] = pose::random_displacement(translation_mag, rotation_mag).transformation_from_world();
			std::cout << j  << " :" << extrema[j].matrix() << std::endl;
		}

		for(int i = -steps; i <= steps; ++i) {
			float t = float(i) / steps;
			
			str << t;

			Eigen::Affine3f I = Eigen::Affine3f::Identity();
			Eigen::Affine3f T;

			for(int j = 0; j < n; ++j) {
				if(i < 0) T = intermediate_transformation(I, extrema[j].inverse(), -t);
				else if(i == 0) T = I;
				else if(i > 0) T = intermediate_transformation(I, extrema[j], t);
			
				float err = (*this)(T);
				str << " " << err;
			}
			
			str << std::endl;
		}
	}
	
	void test_random_ranges_1dim(const std::string& fname, float translation_mag = 1.0, angle rotation_mag = angle::degrees(10.0), int n = 10, int steps = 100) {
		std::ofstream str(fname);
		test_random_ranges_1dim(str, translation_mag, rotation_mag, n, steps);
	}
	
	void test_range_2dim(const std::string& fname, const Eigen::Affine3f& max_tx, const Eigen::Affine3f& max_ty, int steps = 100) const {
		std::ofstream str(fname);
		test_range_2dim(str, max_tx, max_ty, steps);
	}
};


template<typename Correspondences, typename Error_metric>
error_metric_range_tester<Error_metric, Correspondences> make_error_metric_range_tester(const Correspondences& cor, const Error_metric& err) {
	error_metric_range_tester<Error_metric, Correspondences> tester(cor, err);
	return tester;
}

}}

#endif
