#include <iostream>
#include <sqlite3.h>
#include <fstream>
#include <string>
#include <unistd.h>

#include "storage/database.h"
#include "model/server_user.h"
#include "model/quest.h"

bool clearAllTableData(sqlite3* db) {
    const char* get_tables_sql =
        "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, get_tables_sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to get tables: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Begin transaction for better performance
    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* table = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string sql = "DELETE FROM \"" + std::string(table) + "\";";
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to delete from table " << table << ": "
                      << sqlite3_errmsg(db) << std::endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            sqlite3_finalize(stmt);
            return false;
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);

    return true;
}


bool resetDatabase(const char* db_path) {
    std::ofstream ofs(db_path, std::ios::trunc);
    return ofs.good();
}

bool anyTableHasData(sqlite3* db) {
    const char* get_tables_sql =
        "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, get_tables_sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare table list: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* table_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string count_query = "SELECT 1 FROM " + std::string(table_name) + " LIMIT 1;";

        sqlite3_stmt* count_stmt;
        if (sqlite3_prepare_v2(db, count_query.c_str(), -1, &count_stmt, nullptr) == SQLITE_OK) {
            if (sqlite3_step(count_stmt) == SQLITE_ROW) {
                sqlite3_finalize(count_stmt);
                sqlite3_finalize(stmt);
                return true;  // Found data
            }
        }
        sqlite3_finalize(count_stmt);
    }

    sqlite3_finalize(stmt);
    return false;  // No data in any table
}


int main() {

    //std::cout << "Sidequest Server " << std::endl;
    while (1) {

        using namespace Sidequest::Server;
        auto database = new Database();

        database->cleanInit();

        auto user = new ServerUser(database, "crud_user_create@hs-aalen.de", "Temporary User", "");
        user->create_on_database();
        delete(user);

        auto user2 = new ServerUser(database, "crud_user_create@hs-aalen.de");
        user2->read_on_database();


        std::cout << user->display_name << std::endl;
        sleep(2);
    }

    return 0;
}

