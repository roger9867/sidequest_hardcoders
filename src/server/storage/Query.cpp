//
// Created by roger on 5/17/25.
//

#include "Query.h"

#include <iostream>
#include <ostream>

#include "database_exceptions.h"

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
    sqlite3_stmt* Query::get_prepared_statement() {
        return prepared_statement;
    }
    */


    int Query::read_int_value(std::string column_name) {
        int column_index = get_column_index(column_name);
        int result = static_cast<int>(sqlite3_column_int64(prepared_statement, column_index) );
        return result;
    }

    std::string Query::read_text_value(std::string column_name) {
        if (!columnmap || columnmap->find(column_name) == columnmap->end()) {
            throw std::runtime_error("Column name not found: " + column_name);
        }
        int index = (*columnmap)[column_name];
        const unsigned char* text = sqlite3_column_text(prepared_statement, index);

        if (text == nullptr) {
            return "";  // oder optional: throw std::runtime_error(...)
        }

        return reinterpret_cast<const char*>(text);
    }

    std::string Query::get_sql() const {
        return STATEMENT_SQL;
    }

    int Query::execute() {
        int code = -1;
        if (!((code = sqlite3_step(prepared_statement)) == SQLITE_DONE || code == SQLITE_ROW)) {
            reset_statement();
        }
        if (code == SQLITE_ROW && columnmap == nullptr) {
            columnmap = get_column_mapping(); // Existenz der Map garantieren
        }
        if (code == SQLITE_ROW || code == SQLITE_DONE) is_executed = true;
        return code;
    }


    void Query::reset_statement() {
        if (prepared_statement) {
            sqlite3_reset(prepared_statement);
        }
    }


    void Query::prepare() {
        PreparedStatement* statement = database->get_prepared_statement(STATEMENT_SQL);
        if (statement == nullptr) {
            int result = sqlite3_prepare_v2(database->get_handle(), STATEMENT_SQL.c_str(), -1, &statement, nullptr);
            if (result != SQLITE_OK) {
                throw ParameterBindException(STATEMENT_SQL, result);
            }
            database->add_prepared_statement(statement);
        }
        prepared_statement = statement;
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
    void Query::bind_null(int index) {
        sqlite3_bind_null(prepared_statement, index);
    }

    Query::Query(Database* database, std::string statement_sql)
        : database(database), STATEMENT_SQL(statement_sql) {
        prepare();
    }

    Query::~Query() {
        if (prepared_statement) {
            // completely release ressource
            sqlite3_finalize(prepared_statement);
        }
    }

    bool Query::QueryIterator::operator!=(const Query::QueryIterator& other) const {
        return is_end != other.is_end;
    }
    Query::QueryIterator::QueryIterator(Query* query, bool is_end)
    : query(query), is_end(is_end) {
        if (!is_end) {
            if (query->is_executed) {
                sqlite3_reset(query->prepared_statement);
            }

            int code = sqlite3_step(query->prepared_statement);
            query->is_executed = true;

            if (code != SQLITE_ROW) {
                is_end = true;
            } else if (!query->get_columnmap()) {
                query->columnmap = query->get_column_mapping();
            }
        }
    }

    Query::QueryIterator& Query::QueryIterator::operator++() {
        if (!is_end) {
            int code = sqlite3_step(query->prepared_statement);
            if (code != SQLITE_ROW) {
                is_end = true;
            }
        }
        return *this;
    }
    std::unordered_map<std::string, std::string> Query::QueryIterator::operator*() const {
        if (is_end) throw std::out_of_range("Dereferencing end iterator");

        std::unordered_map<std::string, std::string> row;
        auto* columnmap = query->get_columnmap();
        for (const auto& [name, index] : *columnmap) {
            const unsigned char* val = sqlite3_column_text(query->prepared_statement, index);
            row[name] = val ? reinterpret_cast<const char*>(val) : "";
        }
        return row;
    }

    Query::QueryIterator Query::begin() {
        return QueryIterator(this);
    }
    Query::QueryIterator Query::end() {
        return QueryIterator(this, true);
    }

}
