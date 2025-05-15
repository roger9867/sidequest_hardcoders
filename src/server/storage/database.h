#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <unordered_map>

#include <sqlite3.h>
#include "prepared_statement.h"


namespace Sidequest::Server {

	class Persistable;
	class StatementCache;
	class ColumnCache;

	class Database {

	protected:
		const std::string DEFAULT_PATH_TO_DATABASE = "sidequest.db";
		const std::string PATH_TO_SCHEMA = "../create_database.sql";
		const std::string PATH_TO_DATABASE;

		bool is_open = false;
		sqlite3* handle = nullptr;
		friend class StatementCache;

		void open(std::string filepath_of_database);
		void close();
		// read in sql file
		static std::string getSqlFromFile(std::string filepath);

	public:

		Database();
		explicit Database(std::string path_to_database);
		~Database();

		// Clean initialize the Database
		void cleanInit();

		// Database reset
		void recreate();

		StatementCache* statement_cache;
		ColumnCache* column_cache;

		PreparedStatement* prepare(std::string statement_sql);
		void bind(PreparedStatement* prepared_statement, int parameter_index, std::string value);
		void bind(PreparedStatement* prepared_statement, int parameter_index, unsigned int value);
		int execute(PreparedStatement* prepared_statement);
		int execute(std::string sql_statement);
		void reset_statement(PreparedStatement* prepared_statement);
		int read_int_value(PreparedStatement* prepared_statement, std::string column_name);
		std::string read_text_value(PreparedStatement* prepared_statement, std::string column_name);
	};
};

#endif  //DATABASE_H
