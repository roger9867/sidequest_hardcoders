#include <fstream>
#include <sstream>
#include <filesystem>

#include "database.h"
#include "database_exceptions.h"

namespace Sidequest::Server {

	Query* Database::create_query(std::string sql_statement) {
		return new Query(this, sql_statement);
	}

	void Database::clean_init() {
		close();
		recreate();
		init_schema();
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
		statement_cache = new StatementCache( this );
		column_cache = new ColumnCache( this );
	}

	// Create database in custom location
	Database::Database(std::string path_to_database) : PATH_TO_DATABASE(path_to_database) {
		open(PATH_TO_DATABASE);
		init_schema();
		statement_cache = new StatementCache( this );
		column_cache = new ColumnCache( this );
	}

	Database::~Database() {
		if ( is_open )
			close();
		delete statement_cache;
		delete column_cache;
	}

	sqlite3* Database::get_handle() const {
		return handle;
	}

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

	PreparedStatement* Database::get_prepared_statement(const Query& query) const {
		return statement_cache->get_statement(query.get_sql());
	}

	void Database::add_prepared_statement(const Query& query) const {
		statement_cache->add_statement(query.get_sql());
	}
}