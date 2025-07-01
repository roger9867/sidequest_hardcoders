//
// Created by roger on 6/30/25.
//

#ifndef USER_DELETE_COMMAND_H
#include <string>

#include "network/command_handler.h"

namespace Sidequest::Server{

    class Database;

    class UserDeleteCommand: public CommandHandler {
     public:
        UserDeleteCommand(Database* database);
        virtual void execute(const httplib::Request& request, httplib::Response& response) override;
        virtual std::string endpoint() override;
     protected:
        Database* database;
    };

}  // namespace Sidequest::Server

#define USER_DELETE_COMMAND_H
#endif //USER_DELETE_COMMAND_H
