//
// Created by roger on 7/1/25.
//

#ifndef QUEST_BY_PARENT_H
#define QUEST_BY_PARENT_H

#include <string>

#include "network/command_handler.h"

namespace Sidequest::Server {

    class Database;

    class QuestByParentCommand: public CommandHandler {
    public:
        QuestByParentCommand(Database* database);
        virtual void execute(const httplib::Request& request, httplib::Response& response) override;
        virtual std::string endpoint() override;
    protected:
        Database* database;
    };

}  // namespace Sidequest::Server

#endif //QUEST_BY_PARENT_H
