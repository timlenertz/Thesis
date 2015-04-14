#include "results.h"
#include "sqlite3pp.h"
#include "run_result.h"
#include "../pcf_core/util/misc.h"
#include "../pcf_core/image/color_image.h"

#include <iostream>

namespace pcf {
namespace exper {

namespace {
	const std::string default_database_name_ = "pcf_experiment_results.db";
}

struct results::impl {
	sqlite3pp::database database;
	
	impl(const std::string& db) :
		database(db.c_str()) { }
};
		
results::results(const std::string& db) :
	counter_(0)
{
	bool new_database = db.empty() || !file_exists(db);
	impl_.reset(new impl(db.empty() ? default_database_name_ : db));
	if(new_database) create_tables_();
}

results::~results() { }




results::results(results&& res) :
	impl_(std::move(res.impl_)),
	counter_(res.counter_) { }


const void* results::transformation_to_blob_(const Eigen::Affine3f& t) {
	return static_cast<const void*>(t.matrix().data());
}


Eigen::Affine3f results::blob_to_transformation_(const void* raw_data) {
	const float* data = static_cast<const float*>(raw_data);
	Eigen::Matrix4f mat = Eigen::Map<const Eigen::Matrix4f>(data);
	return Eigen::Affine3f(mat);
}



void results::drop_tables_() {
	impl_->database.execute("DROP TABLE IF EXISTS run");
	impl_->database.execute("DROP TABLE IF EXISTS state");
}

void results::create_tables_() {
	drop_tables_();
	
	auto err = impl_->database.execute(
		"CREATE TABLE run ("
			"id INTEGER PRIMARY KEY ASC, "
			"success INTEGER, "
			"actual_success INTEGER, "
			"original_transformation BLOB, "
			"registration_arg REAL, "
			"displacer_arg REAL, "
			"make_fixed_arg REAL, "
			"make_loose_arg REAL, "
			"final_error REAL, "
			"final_actual_error REAL, "
			"final_time INTEGER, "
			"fixed_number_of_points INTEGER, "
			"loose_number_of_points INTEGER, "
			"number_of_states INTEGER"
		")"
	);
	if(err) throw std::runtime_error("Could not create SQLite table 'run'.");
	
	err = impl_->database.execute(
		"CREATE TABLE state ("
			"run_id INTEGER, "
			"step INTEGER, "
			"error REAL, "
			"actual_error REAL, "
			"transformation BLOB, "
			"time INTEGER, "
			"snapshot BLOB, "
			"FOREIGN KEY(run_id) REFERENCES run(id)"
		")"
	);
	if(err) throw std::runtime_error("Could not create SQLite table 'state'.");
}


void results::clear() {
	counter_ = 1;
	impl_->database.execute("DELETE FROM state");
	impl_->database.execute("DELETE FROM run");
}

std::ptrdiff_t results::add(const run_result& rn, bool add_snapshot) {
	sqlite3pp::command insert_run(impl_->database, "INSERT INTO run (id, success, actual_success, original_transformation, registration_arg, displacer_arg, make_fixed_arg, make_loose_arg, final_error, final_actual_error, final_time, fixed_number_of_points, loose_number_of_points, number_of_states) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
	sqlite3pp::command insert_state(impl_->database, "INSERT INTO state (run_id, step, error, actual_error, transformation, time, snapshot) VALUES (?, ?, ?, ?, ?, ?, ?)");

	sqlite3pp::transaction tr(impl_->database);
	{
		insert_run.bind(1, (int)counter_);
		insert_run.bind(2, rn.success ? 1 : 0);
		insert_run.bind(3, rn.actual_success ? 1 : 0);
		insert_run.bind(4, transformation_to_blob_(rn.original_transformation), sizeof(float)*16, false);
		insert_run.bind(5, rn.registration_arg);
		insert_run.bind(6, rn.displacer_arg);
		insert_run.bind(7, rn.make_fixed_arg);
		insert_run.bind(8, rn.make_loose_arg);
		insert_run.bind(9, rn.evolution.back().error);
		insert_run.bind(10, rn.evolution.back().actual_error);
		insert_run.bind(11, (int)rn.evolution.back().time.count());
		insert_run.bind(12, (int)rn.fixed_number_of_points);
		insert_run.bind(13, (int)rn.loose_number_of_points);
		insert_run.bind(14, (int)rn.evolution.size());
		insert_run.execute();
		
		auto run_id = impl_->database.last_insert_rowid();
		
		for(std::ptrdiff_t i = 0; i < rn.evolution.size(); ++i) {
			const run_result::state& st = rn.evolution[i];
			Eigen::Matrix4f transformation = st.transformation.matrix();
			
			insert_state.bind(1, run_id);
			insert_state.bind(2, (int)i);
			insert_state.bind(3, st.error);
			insert_state.bind(4, st.actual_error);
			insert_state.bind(5, transformation_to_blob_(st.transformation), sizeof(float)*16, false);
			insert_state.bind(6, (int)st.time.count());
			
			if(st.snapshot && add_snapshot)
				st.snapshot->export_png_to_memory([&insert_state](const void* buf, std::size_t sz) {
					insert_state.bind(7, buf, (int)sz, false);
				});
			else
				insert_state.bind(7, sqlite3pp::null_type());			
			
			insert_state.execute();
			insert_state.reset();
		}
	}
	auto err = tr.commit();
	if(err) throw std::runtime_error("Could not commit transaction to add experiment run.");
	
	return counter_++;
}


std::size_t results::number_of_runs() const {
	sqlite3pp::query q(impl_->database, "SELECT COUNT(id) FROM run");
	const auto& row = *q.begin();
	return row.get<int>(0);
}


run_result results::operator[](int i) const {
	run_result rn;

	sqlite3pp::query run_query(impl_->database,
		"SELECT success, actual_success, original_transformation, displacer_arg, make_fixed_arg, make_loose_arg, fixed_number_of_points, loose_number_of_points FROM run WHERE id=?"
	);
	run_query.bind(1, i + 1);
	const auto& row = *run_query.begin();
	rn.success = (row.get<int>(0) != 0);
	rn.actual_success = (row.get<int>(1) != 0);
	rn.original_transformation = blob_to_transformation_(row.get<const void*>(2));
	rn.displacer_arg = row.get<float>(3);
	rn.make_fixed_arg = row.get<float>(4);
	rn.make_loose_arg = row.get<float>(5);
	rn.fixed_number_of_points = row.get<int>(6);
	rn.loose_number_of_points = row.get<int>(7);
	
	sqlite3pp::query state_query(impl_->database, 
		"SELECT error, actual_error, transformation, time, snapshot FROM state WHERE run_id=? ORDER BY step ASC"
	);
	state_query.bind(1, i + 1);
	
	
	for(auto it = state_query.begin(); it != state_query.end(); ++it) {
		run_result::state st;
		const auto& row = *it;
		st.error = row.get<float>(0);
		st.actual_error = row.get<float>(1);
		st.transformation = blob_to_transformation_(row.get<const void*>(2));
		st.time = std::chrono::milliseconds( row.get<int>(3) );
		
		const void* snapshot_data = row.get<const void*>(4);
		if(snapshot_data != nullptr) {
			std::size_t sz = row.column_bytes(4);
			color_image img = color_image::import_from_memory(snapshot_data, sz);
			st.snapshot.reset( new color_image(std::move(img)) );
		}
		
		rn.evolution.push_back(std::move(st));
	}
	
	return rn;
}


results::data_point_set results::query(const std::string& query) const {
	results::data_point_set points;
	sqlite3pp::query q(impl_->database, query.c_str());
	
	for(const auto& row : q) {
		float i = row.get<float>(0);
		float o = row.get<float>(1);
		points.emplace_back(i, o);
	}
	return points;
}


}
}
