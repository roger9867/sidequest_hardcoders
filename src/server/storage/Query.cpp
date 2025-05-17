//
// Created by roger on 5/17/25.
//

#include "Query.h"

namespace Sidequest::Server {

    int Query::get_column_index(std::string column_name) {
        return (*columnmap)[column_name];
    }


    ColumnMap* Query::get_columnmap() {
        return columnmap;
    }

    ColumnMap* Query::get_column_mapping() {
        auto column_indices = new std::unordered_map<std::string, int>();
        int columnCount = sqlite3_column_count(prepared_statement);
        for (int i = 0; i < columnCount; ++i) {
            const char* name = sqlite3_column_name(prepared_statement, i);
            if (name) {
                (*column_indices)[std::string(name)] = i;
            }
        }
        return column_indices;
    }


    /*
    int Database::execute(PreparedStatement* prepared_statement) {
        int code = sqlite3_step(prepared_statement);
        return code;
    }

    int Database::execute(std::string sql_statement) {
        int code = sqlite3_exec(handle, sql_statement.c_str(), nullptr, nullptr, nullptr);
        return code;
    }
    */

    int Query::execute() {
    int code = -1;
        if ((code = sqlite3_step(prepared_statement)) != SQLITE_DONE) {
            reset_statement();
        }
        return code;
    }


    void Query::reset_statement() {
        if (prepared_statement) {
            sqlite3_reset(prepared_statement);
        }
    }

    void Query::prepare() {
        int result = sqlite3_prepare_v2(database->get_handle(), STATEMENT_SQL.c_str(), -1, &prepared_statement, nullptr);
        if (result != SQLITE_OK) {
            throw ParameterBindException(STATEMENT_SQL, result);
        }
    }
    void Query::bind(int parameter_index, std::string value) {
        if (prepared_statement == nullptr) {
            prepare();
        }
        int error_code = sqlite3_bind_text(prepared_statement, parameter_index, value.c_str(), -1, SQLITE_TRANSIENT);
        if (error_code != SQLITE_OK) {
            throw ParameterBindException("error binding parameter " + std::to_string(parameter_index) + " to " + value, error_code);
        }
    }
    void Query::bind(int parameter_index, unsigned int value) {
        if (prepared_statement == nullptr) {
            prepare();
        }
        int error_code = sqlite3_bind_int(prepared_statement, parameter_index, value );
        if (error_code != SQLITE_OK) {
            sqlite3_finalize(prepared_statement);
            throw ParameterBindException("error binding parameter " + std::to_string(parameter_index) + " to " + std::to_string(value), error_code);
        }
    }

    Query::Query(Database* database, std::string statement_sql)
        : database(database), STATEMENT_SQL(statement_sql) {
        prepare();
    }

    Query::~Query() {
        if (prepared_statement) {
            sqlite3_finalize(prepared_statement);
        }
    }


}
