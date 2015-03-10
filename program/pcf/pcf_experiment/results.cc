#include "results.h"
#include "sqlite3pp.h"
#include "../pcf_core/util/misc.h"

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
	impl_(new impl(db.empty() ? default_database_name_ : db)),
	counter_(0)
{
	if(db.empty() || !file_exists(db)) create_tables_();
}

results::~results() { }

const void* results::transformation_to_blob_(const Eigen::Affine3f& t) {
	return static_cast<const void*>(t.matrix().data());
}


results::results(results&& res) :
	impl_(std::move(res.impl_)),
	counter_(res.counter_) { }



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
			"original_transformation BLOB, "
			"displacer_arg REAL, "
			"fixed_modifier_arg REAL, "
			"loose_modifier_arg REAL, "
			"final_error REAL, "
			"final_actual_error REAL, "
			"final_time INTEGER, "
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
			"FOREIGN KEY(run_id) REFERENCES run(id)"
		")"
	);
	if(err) throw std::runtime_error("Could not create SQLite table 'state'.");
}


void results::clear() {
	counter_ = 1;
	impl_->database.execute("DELETE FRUN state");
	impl_->database.execute("DELETE FROM run");
}

void results::add(const run& rn) {
	sqlite3pp::command insert_run(impl_->database, "INSERT INTO run (id, success, original_transformation, displacer_arg, fixed_modifier_arg, loose_modifier_arg, final_error, final_actual_error, final_time, number_of_states) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
	sqlite3pp::command insert_state(impl_->database, "INSERT INTO state (run_id, step, error, actual_error, transformation, time) VALUES (?, ?, ?, ?, ?, ?)");

	sqlite3pp::transaction tr(impl_->database);
	{
		insert_run.bind(1, (int)counter_);
		insert_run.bind(2, rn.success ? 1 : 0);
		insert_run.bind(3, transformation_to_blob_(rn.original_transformation), sizeof(float)*16, false);
		insert_run.bind(4, rn.displacer_arg);
		insert_run.bind(5, rn.fixed_modifier_arg);
		insert_run.bind(6, rn.loose_modifier_arg);
		insert_run.bind(7, rn.evolution.back().error);
		insert_run.bind(8, rn.evolution.back().actual_error);
		insert_run.bind(9, (int)rn.evolution.back().time.count());
		insert_run.bind(10, (int)rn.evolution.size());
		insert_run.execute();
		
		auto run_id = impl_->database.last_insert_rowid();
		
		for(std::ptrdiff_t i = 0; i < rn.evolution.size(); ++i) {
			const state& st = rn.evolution[i];
			Eigen::Matrix4f transformation = st.transformation.matrix();
			
			insert_state.bind(1, run_id);
			insert_state.bind(2, (int)i);
			insert_state.bind(3, st.error);
			insert_state.bind(4, st.actual_error);
			insert_state.bind(5, transformation_to_blob_(st.transformation), sizeof(float)*16, false);
			insert_state.bind(6, (int)st.time.count());
			insert_state.execute();
			insert_state.reset();
		}
	}
	auto err = tr.commit();
	if(err) throw std::runtime_error("Could not commit transaction to add experiment run.");
	
	++counter_;
}


std::size_t results::number_of_runs() const {
	sqlite3pp::query q(impl_->database, "SELECT COUNT(id) FROM run");
	const auto& row = *q.begin();
	return row.get<int>(0);
}


results::run results::operator[](int i) const {
	run rn;

	sqlite3pp::query run_query(impl_->database,
		"SELECT success, original_transformation, displacer_arg, fixed_modifier_arg, loose_modifier_arg FROM run WHERE id=?"
	);
	run_query.bind(1, i + 1);
	const auto& row = *run_query.begin();
	rn.success = (row.get<int>(0) != 0);
	rn.original_transformation = blob_to_transformation_(row.get<const void*>(1));
	rn.displacer_arg = row.get<float>(2);
	rn.fixed_modifier_arg = row.get<float>(3);
	rn.loose_modifier_arg = row.get<float>(4);
	
	sqlite3pp::query state_query(impl_->database, 
		"SELECT step, error, actual_error, transformation, time FROM state WHERE run_id=? ORDER BY step ASC"
	);
	state_query.bind(1, (int)i);
	
	
	for(auto it = state_query.begin(); it != state_query.end(); ++it) {
		state st;
		const auto& row = *it;
		st.error = row.get<float>(0);
		st.actual_error = row.get<float>(1);
		st.transformation = blob_to_transformation_(row.get<const void*>(2));
		st.time = std::chrono::milliseconds( row.get<int>(3) );
		
		rn.evolution.push_back(st);
	}
	
	return rn;
}


results::data_point_set results::scatterplot(input_variable iv, output_variable ov, bool success_only) const {
	std::string query = "SELECT ";
	switch(iv) {
		case fixed_modifier_arg: query += "fixed_modifier_arg"; break;
		case loose_modifier_arg: query += "loose_modifier_arg"; break;
		case displacer_arg: query += "displacer_arg"; break;
		default: throw std::invalid_argument("Invalid input variable.");
	}
	query += ", ";
	switch(ov) {
		case success: query += "success"; break;
		case final_error: query += "final_error"; break;
		case final_actual_error: query += "final_actual_error"; break;
		case final_time: query += "final_time"; break;
		case number_of_states: query += "number_of_states"; break;
		default: throw std::invalid_argument("Invalid output variable.");
	}
	if(success_only) query += " WHERE success";
	query += " FROM run";
	
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
