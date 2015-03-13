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

class color_image;

namespace exper {

class run_result;

/**
Results from experiment.
Stores result from each run, with its states. The results get stored into an SQLite database, and can be converted to and from the structs.
Provides functionality to extract two-variable relationships from the database.
*/
class results {
public:
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
	
	color_image blob_to_color_image_(const void*, std::size_t);

public:
	explicit results(const std::string& db = "");
	results(const results&) = delete;
	results(results&&);
	~results();
	
	results& operator=(const results&) = delete;
	results& operator=(results&&) = delete;
	
	void clear();
	void add(const run_result&);
	
	std::size_t number_of_runs() const;
	run_result operator[](int) const;
	
	data_point_set query(const std::string& query) const;
};

}
}

#endif
