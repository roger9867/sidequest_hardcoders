//
// Created by roger on 6/30/25.
//

#ifndef QUEST_CREATE_COMMAND_H
#include <string>

#include "network/command_handler.h"

namespace Sidequest::Server {

    class Database;

    class QuestCreateCommand: public CommandHandler {
     public:
        QuestCreateCommand(Database* database);
        virtual void execute(const httplib::Request& request, httplib::Response& response) override;
        virtual std::string endpoint() override;
     protected:
        Database* database;
    };

}  // namespace Sidequest::Server

#define QUEST_CREATE_COMMAND_H

#endif //QUEST_CREATE_COMMAND_H
