//
// Created by roger on 5/21/25.
//

#ifndef COLUMN_CACHE_H
#define COLUMN_CACHE_H

#include <string>
#include <unordered_map>
#include "Query.h"



namespace Sidequest::Server {

    typedef sqlite3_stmt PreparedStatement;
    typedef std::unordered_map<std::string, int> ColumnMap;

        class Database;



        class ColumnCache {
        public:
            typedef std::unordered_map<std::string, int> ColumnMap;

            ColumnCache( Database* database );
            virtual ~ColumnCache();

            //int get_column_index(PreparedStatement* statement, std::string column_name);
            ColumnMap* get_columns_of_statement(PreparedStatement* statement);
            ColumnMap* add_columns_of_statement(PreparedStatement* statement);

        protected:
            std::unordered_map<PreparedStatement*, ColumnMap*> columns_by_statement;
            Database* database;
        };
}



#endif //COLUMN_CACHE_H
