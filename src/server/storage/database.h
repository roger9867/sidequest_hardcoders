#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <unordered_map>
#include <sqlite3.h>
#include "Query.h"
#include "column_cache.h"
#include "statement_cache.h"


namespace Sidequest::Server {

	class Persistable;
	class StatementCache;
	class ColumnCache;
	class Query;

	class Database {
		friend class StatementCache;
		friend class ColumnCache;

	protected:
		const std::string DEFAULT_PATH_TO_DATABASE = "sidequest.db";
		const std::string PATH_TO_SCHEMA = "../create_database.sql";
		const std::string PATH_TO_DATABASE;

		StatementCache* statement_cache;
		ColumnCache* column_cache;

		bool is_open = false;
		sqlite3* handle = nullptr;

		void open(std::string filepath_of_database);
		void close();
		static std::string get_sql_from_file(std::string filepath);

		int init_schema();

	public:

		Database();
		explicit Database(std::string path_to_database);
		~Database();

		// clean initialize database
		void clean_init();

		// reset database
		void recreate();
		Query* create_query(std::string sql_statement);

		sqlite3* get_handle() const;
		PreparedStatement* get_prepared_statement(const Query& query) const;
		void add_prepared_statement(const Query& query) const;
		ColumnCache* get_column();
	};
};

#endif  //DATABASE_H
