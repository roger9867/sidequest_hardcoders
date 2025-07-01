//
// Created by roger on 6/30/25.
//

#ifndef QUEST_READ_COMMAND_H
#include <string>

#include "network/command_handler.h"

namespace Sidequest::Server {

    class Database;

    class QuestReadCommand: public CommandHandler {
     public:
        QuestReadCommand(Database* database);
        virtual void execute(const httplib::Request& request, httplib::Response& response) override;
        virtual std::string endpoint() override;
     protected:
        Database* database;
    };

}  // namespace Sidequest::Server

#define QUEST_READ_COMMAND_H
#endif //QUEST_READ_COMMAND_H
