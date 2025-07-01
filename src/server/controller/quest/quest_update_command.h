//
// Created by roger on 6/30/25.
//

#ifndef QUEST_UPDATE_H
#include <string>

#include "network/command_handler.h"

namespace Sidequest::Server {

    class Database;

    class QuestUpdateCommand: public CommandHandler {
     public:
        QuestUpdateCommand(Database* database);
        virtual void execute(const httplib::Request& request, httplib::Response& response) override;
        virtual std::string endpoint() override;
     protected:
        Database* database;
    };

}  // namespace Sidequest::Server

#define QUEST_UPDATE_H
#endif //QUEST_UPDATE_H
