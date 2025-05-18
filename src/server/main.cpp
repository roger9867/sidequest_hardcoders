#include <iostream>
#include <sqlite3.h>
#include <fstream>
#include <string>


#include "storage/database.h"
#include "model/server_user.h"

int main() {

    //std::cout << "Sidequest Server " << std::endl;



    using namespace Sidequest::Server;
    auto database = new Database();

    database->clean_init();


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


    return 0;
}
