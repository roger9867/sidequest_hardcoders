#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <unordered_map>

#include <sqlite3.h>
#include "Query.h"


namespace Sidequest::Server {

	class Persistable;
	class StatementCache;
	class ColumnCache;
	class Query;

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

		sqlite3* get_handle();
		Query* create_query(std::string sql_statement);
	};
};

#endif  //DATABASE_H
