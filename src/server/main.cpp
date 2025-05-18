#include <iostream>
#include <sqlite3.h>
#include <fstream>
#include <string>
#include <unistd.h>

#include "storage/database.h"
#include "model/server_user.h"
#include "model/quest.h"
#include "model/server_quest.h"

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



    using namespace Sidequest::Server;
    auto database = new Database();

    database->clean_init();

    /*
    auto user1 = new ServerUser(database, "crud_user1_create@hs-aalen.de", "Temporary User1", "1");
    user1->create_on_database();
    auto user2 = new ServerUser(database, "crud_user2_create@hs-aalen.de", "Temporary User2", "2");
    user2->create_on_database();
    auto user3 = new ServerUser(database, "crud_user3_create@hs-aalen.de", "Temporary User3", "3");
    user3->create_on_database();
    auto user4 = new ServerUser(database, "crud_user4_create@hs-aalen.de", "Temporary User4", "4");
    user4->create_on_database();

    using namespace Sidequest::Server;
    Query* all_users = database->create_query("SELECT * FROM user u WHERE u.password > 2 ");
    all_users->execute();
    for (auto it = all_users->begin(); it != all_users->end(); ++it) {

        auto row = *it;
        std::cout << row["email"] << ", " << row["display_name"] << ", " << row["password"] << std::endl;
    }
    */


    auto quest1 = new ServerQuest(database, "id1", "caption1", nullptr, nullptr );
    auto questP = new ServerQuest(database, "idP", "captionP", nullptr, nullptr );

    quest1->set_parent(questP);
    questP->add_subquest(quest1);


    quest1->create_on_database();
    questP->create_on_database();
    delete quest1;
    delete questP;

    auto quest2 = new ServerQuest(database, "id1");
    quest2->read_on_database();

    std::cout << quest2->get_id() << ", " << quest2->get_caption() << ", " << quest2->get_parent()->get_id()  << std::endl;

    std::cout << "$$" << std::endl;
    ServerQuest* quest2P = new ServerQuest(database, quest2->get_parent()->get_id());
    std::cout << "$$" << std::endl;
    std::cout << quest2P->get_id() << ", " << quest2P->get_caption() << ", " << quest2P->get_parent()  << std::endl;

    quest2P->read_on_database();
    std::cout << quest2P->get_id() << ", " << quest2P->get_caption() << ", " << quest2P->get_parent()  << std::endl;

    /*
    auto quest2 = new ServerQuest(database, "id2", "caption2", nullptr, nullptr );
    quest2->create_on_database();
    auto quest3 = new ServerQuest(database, "id3", "caption3", nullptr, nullptr );
    quest3->create_on_database();
    auto quest4 = new ServerQuest(database, "id4", "caption4", nullptr, nullptr );
    quest4->create_on_database();
    */

    /*
    using namespace Sidequest::Server;
    Query* all_quests = database->create_query("SELECT * FROM quest");
    all_quests->execute();
    for (auto it = all_quests->begin(); it != all_quests->end(); ++it) {
        auto row = *it;
        std::cout << row["id"] << ", " << row["caption"] << ", " << row["parent"] << std::endl;
    }
    */

    return 0;
}
