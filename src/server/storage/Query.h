//
// Created by roger on 5/17/25.
//

#ifndef QUERY_H
#define QUERY_H
#include <string>
#include <unordered_map>

#include "database_exceptions.h"
#include "database.h"
#include "sqlite3.h"

namespace Sidequest::Server {

    typedef sqlite3_stmt PreparedStatement;
    typedef std::unordered_map<std::string, int> ColumnMap;

    class Database;

    class Query {


    public:
        class QueryIterator {

        public:
            explicit QueryIterator(Query* query, bool is_end = false);

            QueryIterator& operator++();           // Prefix increment

            bool operator!=(const QueryIterator& other) const;

            std::unordered_map<std::string, std::string> operator*() const;


        private:
            Query* query;
            bool is_end;
        };

        QueryIterator begin();
        QueryIterator end();


    protected:
        const std::string STATEMENT_SQL;
        Database* database = nullptr;
        PreparedStatement* prepared_statement = nullptr;
        ColumnMap* columnmap = nullptr;

        int get_column_index(std::string column_name);

        ColumnMap* get_columnmap();
        ColumnMap* get_column_mapping();

        void prepare();
        void reset_statement();

        bool is_executed = 0;


    public:
        Query(Database* database, std::string statement_sql);
        ~Query();

        int read_int_value(std::string column_name);
        std::string read_text_value(std::string column_name);

        int execute(/*PreparedStatement* prepared_statement*/);
        //int execute(std::string sql_statement);

        void bind(int parameter_index, std::string value);
        void bind(int parameter_index, unsigned int value);
        sqlite3_stmt * get_prepared_statement();
    };
}

#endif //QUERY_H


