//
// Created by roger on 5/21/25.
//

#include "statement_cache.h"

namespace Sidequest::Server {

        StatementCache::StatementCache(Database* database)
            : database(database) {}

        StatementCache::~StatementCache() {}

        PreparedStatement* StatementCache::get_statement(std::string statement_sql) {
            auto statement = prepared_statements.find(statement_sql);
            if (statement == prepared_statements.end())
                return nullptr;
            return statement->second;
        }

        PreparedStatement* StatementCache::add_statement(std::string statement_sql) {
            PreparedStatement* statement;
            auto result = sqlite3_prepare_v2(database->handle, statement_sql.c_str(), -1, &statement, nullptr);
            if (result != SQLITE_OK) {
                throw ParameterBindException( statement_sql, result );
            }
            prepared_statements[statement_sql] = statement;
            return statement;
        }
}