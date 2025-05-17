#include <fstream>
#include <sstream>
#include <filesystem>

#include "database.h"
#include "database_exceptions.h"
//#include "statement_cache.h"
//#include "column_cache.h"

namespace Sidequest::Server {

	Query* Database::create_query(std::string sql_statement) {
		return new Query(this, sql_statement);
	}

	void Database::clean_init() {
		//open(PATH_TO_DATABASE);
		close();
		recreate();
		init_schema();
		//create_query(get_sql_from_file(PATH_TO_SCHEMA))->execute();
		//execute(getSqlFromFile(PATH_TO_SCHEMA));
		//statement_cache = new StatementCache(this);
		//column_cache = new ColumnCache(this);
	}

	int Database::init_schema() {
		std::string sql = get_sql_from_file(PATH_TO_SCHEMA);
		int code = sqlite3_exec(handle, sql.c_str(), nullptr, nullptr, nullptr);
		return code;
	}

	void Database::recreate() {
		if (is_open) close();
		if (std::filesystem::exists(PATH_TO_DATABASE)) {
			std::filesystem::remove(PATH_TO_DATABASE);
		}
		if (sqlite3_open(PATH_TO_DATABASE.c_str(), &handle) != SQLITE_OK){
			throw std::runtime_error("Failed to create fresh database: " + std::string(sqlite3_errmsg(handle)));
		}
		is_open = true;
	}

	// read in sql file
	std::string Database::get_sql_from_file(std::string path_to_sql_file) {
		std::ifstream file(path_to_sql_file);
		if (!file.is_open()) {
			throw std::runtime_error("Cannot open SQL file: " + path_to_sql_file);
		}
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	// Default constructor creates new database in default location
	Database::Database() {
		open(DEFAULT_PATH_TO_DATABASE);
		init_schema();
		//create_query(get_sql_from_file(PATH_TO_SCHEMA))->execute();
		//execute(getSqlFromFile(PATH_TO_SCHEMA));
		//statement_cache = new StatementCache(this);
		//column_cache = new ColumnCache(this);
	}

	// Create database in custom location
	Database::Database(std::string path_to_database) : PATH_TO_DATABASE(path_to_database) {
		open(PATH_TO_DATABASE);
		init_schema();
		//create_query(get_sql_from_file(PATH_TO_SCHEMA))->execute();
		//execute(getSqlFromFile(PATH_TO_SCHEMA));
		//statement_cache = new StatementCache(this);
		//column_cache = new ColumnCache(this);
	}

	Database::~Database() {
		//delete(statement_cache);
		//delete(column_cache);
		if ( is_open )
			close();
	}

	sqlite3* Database::get_handle() {
		return handle;
	}

	/*
	PreparedStatement* Database::prepare(std::string statement_sql) {
		PreparedStatement* prepared_statement = statement_cache->get_statement( statement_sql );
		if (prepared_statement == nullptr) {
			prepared_statement = statement_cache->add_statement(statement_sql);
		}
		return prepared_statement;
	}
	*/



	/*
	int Database::execute(PreparedStatement* prepared_statement) {
		int code = sqlite3_step(prepared_statement);
		return code;
	}

	int Database::execute(std::string sql_statement) {
		int code = sqlite3_exec(handle, sql_statement.c_str(), nullptr, nullptr, nullptr);
		return code;
	}
	*/



	/*
	void Database::reset_statement(PreparedStatement* prepared_statement) {
		sqlite3_reset(prepared_statement);
	}
	*/

	/*
	int Database::read_int_value(PreparedStatement* statement, std::string column_name) {
		int column_index = column_cache->get_column_index( statement, column_name);
		int result = static_cast<int>( sqlite3_column_int64( statement, column_index) );
		return result;
	}

	std::string Database::read_text_value(PreparedStatement* statement, std::string column_name) {
		int column_index = column_cache->get_column_index(statement, column_name);
		auto c_str = reinterpret_cast<const char*>( sqlite3_column_text(statement, column_index) );
		std::string result( c_str );
		return result;
	}
	*/







	void Database::open(std::string path_of_database) {
		int return_code = sqlite3_open( path_of_database.c_str(), &handle );
		if (return_code != SQLITE_OK) {
			sqlite3_close( handle );
			throw DatabaseNotFoundException( std::string("database not found: ") + path_of_database );
		}
		is_open = true;
	}

	void Database::close() {
		sqlite3_close( handle );
		is_open = false;
	}
}