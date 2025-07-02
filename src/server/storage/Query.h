//
// Created by roger on 5/17/25.
//

#ifndef QUERY_H
#define QUERY_H
#include <string>
#include <unordered_map>


#include "database.h"
#include "sqlite3.h"
#include "../sidequest/model/id.h"
#include "prepared_statement.h"
#include "model/server_quest.h"

namespace Sidequest::Server {

    class Database;

    class Query {

    public:
        class QueryIterator {
        public:
            explicit QueryIterator(Query* query, bool is_end = false);
            QueryIterator& operator++();
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
        void prepare();
        bool is_executed = 0;

    public:

        Query(Database* database, std::string statement_sql);
        ~Query();

        void bind(int parameter_index, std::string value);
        void bind(int parameter_index, unsigned int value);
        void bind_null(int index);

        int read_int_value(std::string column_name);
        std::string read_text_value(std::string column_name);
        int execute();
        PreparedStatement* get_prepared_statement() const;
        std::string get_sql() const;
        void reset_statement();
    };
}

#endif //QUERY_H


