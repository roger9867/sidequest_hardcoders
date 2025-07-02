#include <iostream>
#include <sqlite3.h>
#include <fstream>
#include <string>


#include "controller/quest/quest_create_command.h"
#include "controller/quest/quest_delete_command.h"
#include "controller/quest/quest_read_command.h"
#include "controller/quest/quest_update_command.h"
#include "controller/user/user_create_command.h"
#include "controller/user/user_delete_command.h"
#include "controller/user/user_read_command.h"
#include "controller/user/user_update_command.h"
#include "storage/database.h"
#include "model/server_user.h"

#include "network/network_handler.h"
#include "network/command_handler.h" // Beispiel

int main() {

    auto database = new Sidequest::Server::Database();
    Sidequest::Server::ConnectionHandler server("localhost", 4000);

    server.register_options_handler([](const httplib::Request& req, httplib::Response& res) {
        std::cout << "OPTIONS request for " << req.path << std::endl;
    res.set_header("Access-Control-Allow-Origin", "http://localhost:3001");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
    res.status = 200;
    });



    auto* user_create_handler = new Sidequest::Server::UserCreateCommand(database);
    auto* user_delete_handler = new Sidequest::Server::UserDeleteCommand(database);
    auto* user_read_handler = new Sidequest::Server::UserReadCommand(database);
    auto* user_update_handler = new Sidequest::Server::UserUpdateCommand(database);

    auto* quest_create_handler = new Sidequest::Server::QuestCreateCommand(database);
    auto* quest_delete_handler = new Sidequest::Server::QuestDeleteCommand(database);
    auto* quest_read_handler = new Sidequest::Server::QuestReadCommand(database);
    auto* quest_update_handler = new Sidequest::Server::QuestUpdateCommand(database);
    auto* quest_by_parent_handler = new Sidequest::Server::QuestUpdateCommand(database);
    auto* quest_by_owner_handler = new Sidequest::Server::QuestUpdateCommand(database);



    server.register_get_command(user_read_handler);
    server.register_get_command(quest_read_handler);
    server.register_get_command(quest_by_parent_handler);
    server.register_get_command(quest_by_owner_handler);

    server.register_post_command(user_create_handler);
    server.register_post_command(quest_create_handler);

    server.register_delete_command(user_delete_handler);
    server.register_delete_command(quest_delete_handler);

    server.register_put_command(user_update_handler);
    server.register_put_command(quest_update_handler);

    std::cout << "Starting server ..." << std::endl;

    server.listen();
    return 0;
}
