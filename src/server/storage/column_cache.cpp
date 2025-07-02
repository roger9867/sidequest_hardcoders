//
// Created by roger on 5/21/25.
//

#include <iostream>

#include "column_cache.h"
#include "database.h"

namespace Sidequest::Server {

        ColumnCache::ColumnCache(Database* database)
            : database(database) {}

        ColumnCache::~ColumnCache() {}

        ColumnCache::ColumnMap* ColumnCache::get_columns_of_statement(PreparedStatement* statement) {
            auto entry = columns_by_statement.find(statement);
            if (entry == columns_by_statement.end())
                return add_columns_of_statement( statement );
            return entry->second;
        }

        ColumnCache::ColumnMap* ColumnCache::add_columns_of_statement(PreparedStatement* statement) {
            auto column_indices = new std::unordered_map<std::string, int>();
            int columnCount = sqlite3_column_count( statement );
            for (int i = 0; i < columnCount; ++i) {
                const char* name = sqlite3_column_name( statement, i );
                if (name) {
                    (*column_indices)[std::string( name ) ] = i;
                }
            }
            columns_by_statement[statement] = column_indices;
            return column_indices;
        }
    }
