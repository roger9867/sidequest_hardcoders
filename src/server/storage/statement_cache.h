//
// Created by roger on 5/21/25.
//

#ifndef STATEMENT_CACHE_H
#define STATEMENT_CACHE_H

#include <string>
#include <unordered_map>

#include "Query.h"
#include "statement_cache.h"
#include "database.h"
#include "database_exceptions.h"

namespace Sidequest::Server {
        class Database;
        class Query;

        class StatementCache {
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

#endif //STATEMENT_CACHE_H
