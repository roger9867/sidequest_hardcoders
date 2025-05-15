#ifndef STATEMENT_CACHE_H
#define STATEMENT_CACHE_H

#include <string>
#include <unordered_map>

#include "prepared_statement.h"

namespace Sidequest::Server {

	class Database;

	class StatementCache {
	 protected:
		std::unordered_map<std::string, PreparedStatement*> prepared_statements;
		Database* database;

	 public:
		StatementCache(Database* database);
		virtual ~StatementCache();

		PreparedStatement* get_statement(std::string statement_sql);
		PreparedStatement* add_statement(std::string statement_sql);
	};
}

#endif  //STATEMENT_CACHE_H
