#pragma once

#include <string>
#include <unordered_map>

#include "prepared_statement.h"

namespace Sidequest
{
	namespace Server
	{
		class Database;

		class ColumnCache
		{
		public:
			typedef std::unordered_map<std::string, int> ColumnMap;

			ColumnCache( Database* database );
			virtual ~ColumnCache();

			int get_column_index(PreparedStatement* statement, std::string column_name);
			ColumnMap* get_columns_of_statement(PreparedStatement* statement);
			ColumnMap* add_columns_of_statement(PreparedStatement* statement);

		protected:
			std::unordered_map<PreparedStatement*, ColumnMap*> columns_by_statement;
			Database* database;
		};

	};
}