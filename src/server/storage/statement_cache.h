#pragma once

#include <string>
#include <unordered_map>

#include "prepared_statement.h"

namespace Sidequest
{
	namespace Server
	{
		class Database;

		class StatementCache
		{
		public:
			StatementCache( Database* database );
			virtual ~StatementCache();

			PreparedStatement* get_statement(std::string statement_sql);
			PreparedStatement* add_statement(std::string statement_sql);

		protected:
			std::unordered_map<std::string, PreparedStatement*> prepared_statements;
			Database* database;
		};

	};
}