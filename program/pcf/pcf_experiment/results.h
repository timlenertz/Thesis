#ifndef PCFEX_RESULTS_H_
#define PCFEX_RESULTS_H_

#include <chrono>
#include <vector>
#include <Eigen/Geometry>
#include <memory>
#include <utility>
#include <string>
#include "../pcf_core/util/coordinates.h"

namespace pcf {
namespace exper {

/**
Results from experiment.
Stores result from each run, with its states. The results get stored into an SQLite database, and can be converted to and from the structs.
Provides functionality to extract two-variable relationships from the database.
*/
class results {
public:
	/// State during one registration run.
	struct state {
		float error; ///< Error metric at this state, as measured by the used correspondences algorithm.
		float actual_error; ///< Error metric at this state, measured knowing the true point correspondences.
		Eigen::Affine3f transformation; ///< Transformation at this state.
		std::chrono::milliseconds time; ///< Elapsed time since start of run.
	};
	
	/// Results from one registration run.
	struct run {
		std::vector<state> evolution; ///< States of the registration.
		bool success; ///< Whether the registration was deemed successful.
		Eigen::Affine3f original_transformation; ///< Initial transformation applied to loose point cloud.
		float displacer_arg; ///< Argument for displacer.
		float fixed_modifier_arg; ///< Argument for fixed point cloud modifier.
		float loose_modifier_arg; ///< Argument for loose point cloud modifier.
	};
	
	enum input_variable {
		fixed_modifier_arg,
		loose_modifier_arg,
		displacer_arg,
	};
	
	enum output_variable {
		success,
		final_error,
		final_actual_error,
		final_time,
		number_of_states
	};
	
	using data_point = coordinates<2, float>;
	using data_point_set = std::vector<data_point>;

private:
	// PIMPL: Use sqlite3pp only in .cc
	struct impl;
	std::unique_ptr<impl> impl_;
	std::size_t counter_;
	
	void drop_tables_();
	void create_tables_();
	
	static const void* transformation_to_blob_(const Eigen::Affine3f&);
	static Eigen::Affine3f blob_to_transformation_(const void*);

public:
	explicit results(const std::string& db = "");
	results(const results&) = delete;
	results(results&&);
	~results();
	
	results& operator=(const results&) = delete;
	results& operator=(results&&) = delete;
	
	void clear();
	void add(const run&);
	
	std::size_t number_of_runs() const;
	run operator[](int) const;
	
	data_point_set scatterplot(input_variable, output_variable, bool success_only) const;
};

}
}

#endif
