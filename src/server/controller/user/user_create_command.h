//
// Created by roger on 6/30/25.
//

#ifndef USER_CREATE_COMMAND_CPP_H
#include <string>

#include "network/command_handler.h"

namespace Sidequest::Server {

    class Database;

    class UserCreateCommand: public CommandHandler {
     public:
        UserCreateCommand(Database* database);
        virtual void execute(const httplib::Request& request, httplib::Response& response) override;
        virtual std::string endpoint() override;
     protected:
        Database* database;
    };

}  // namespace Sidequest::Server

#define USER_CREATE_COMMAND_CPP_H
#endif //USER_CREATE_COMMAND_CPP_H
