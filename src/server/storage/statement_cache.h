//
// Created by roger on 5/21/25.
//

#ifndef STATEMENT_CACHE_H
#define STATEMENT_CACHE_H

#include <mutex>
#include <string>
#include <unordered_map>

#include "Query.h"
#include "database.h"
#include "database_exceptions.h"
#include "prepared_statement.h"

namespace Sidequest::Server {

        class Database;
        class Query;

        class StatementCache {
            //struct CachedStatement {
            //CachedStatement* cached_statement;
              //  std::mutex cache_mutex = std::mutex();
            //};
        public:
            StatementCache(Database* database);
            virtual ~StatementCache();

            PreparedStatement* get_statement(const std::string& statement_sql);
            PreparedStatement* add_statement(const std::string& statement_sql);
            void release_statement(PreparedStatement* statement);

        protected:
            std::unordered_map<std::string, PreparedStatement*> cached_statements;
            Database* database;
        };
};

#endif //STATEMENT_CACHE_H
