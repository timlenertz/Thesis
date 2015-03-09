#include "results.h"
#include "sqlite3pp.h"


namespace pcf {

namespace {
	const std::string database_name_ = "pcf_experiment_results.db";
	const std::string insert_run_command_ = "INSERT INTO run (success, original_transformation, fixed_modifier_arg, loose_modifier_arg) VALUES (?, ?, ?, ?)";
	const std::string insert_state_command_ = "INSERT INTO state (run_id, step, error, actual_error, transformation, time) VALUES (?, ?, ?, ?, ?, ?)";
}

struct experiment_results::impl {
	sqlite3pp::database database;
	
	impl() :
		database(database_name_.c_str()) { }
};


experiment_results::experiment_results() :
	impl_(new impl),
	counter_(0) { }
{
	create_tables_();
}

experiment_results::~experiment_results() { }


void experiment_results::drop_tables_() {
	impl_->database.execute("DROP TABLE IF EXISTS run");
	impl_->database.execute("DROP TABLE IF EXISTS state");
}

void experiment_results::create_tables_() {
	drop_tables_();
	auto err = impl_->database.execute(
		"CREATE TABLE run ("
			"id INTEGER PRIMARY KEY ASC, "
			"index INTEGER NOT NULL UNIQUE, "
			"success INTEGER NOT NULL, "
			"original_transformation BLOB NOT NULL, "
			"fixed_modifier_arg REAL NOT NULL, "
			"loose_modifier_arg REAL NOT NULL"
		")"
	);
	if(err) throw std::runtime_error("Could not create SQLite table 'run'.");
	
	err = impl_->database.execute(
		"CREATE TABLE state ("
			"run_id INTEGER NOT NULL, "
			"step INTEGER NOT NULL, "
			"error REAL NOT NULL, "
			"actual_error REAL NOT NULL, "
			"transformation BLOB NOT NULL, "
			"time INTEGER NOT NULL, "
			"FOREIGN KEY(run_id) REFERENCES run(id)"
		")"
	);
	if(err) throw std::runtime_error("Could not create SQLite table 'state'.");
}


void experiment_results::clear() {
	counter_ = 0;
}

void experiment_results::add(const run& rn) {
	sqlite3pp::command insert_run(impl_->database, insert_run_command_.c_str());
	sqlite3pp::command insert_state(impl_->database, insert_state_command_.c_str());

	sqlite3pp::transaction tr(impl_->database);
	{
		Eigen::Matrix4f original_transformation = rn.original_transformation.matrix();
		insert_run.bind(1, (int)counter_);
		insert_run.bind(2, rn.success ? 1 : 0);
		insert_run.bind(3, static_cast<const void*>(original_transformation.data()), sizeof(float)*16, false);
		insert_run.bind(4, rn.fixed_modifier_arg);
		insert_run.bind(5, rn.loose_modifier_arg);
		insert_run.execute();
		
		auto run_id = impl_->database.last_insert_rowid();
		
		for(std::ptrdiff_t i = 0; i < rn.evolution.size(); ++i) {
			const state& st = rn.evolution[i];
			Eigen::Matrix4f transformation = st.transformation.matrix();
			
			insert_state.bind(1, run_id);
			insert_state.bind(2, (int)i);
			insert_state.bind(3, st.error);
			insert_state.bind(4, st.actual_error);
			insert_state.bind(5, static_cast<const void*>(transformation.data()), sizeof(float)*16, false);
			insert_state.bind(6, st.time.count());
			insert_state.execute();
			insert_state.reset();
		}
	}
	auto err = tr.commit();
	if(err) throw std::runtime_error("Could not commit transaction to add experiment run.");
	
	++counter_;
}


std::size_t number_of_runs() const {
	sqlite3pp::query q("SELECT COUNT(id) FROM run");
	return q.begin()->get<std::size_t>(1);
}


run operator[](std::ptrdiff_t i) const {
	sqlite3pp::query q("SELECT * FROM run WHERE index=?", );
}


}