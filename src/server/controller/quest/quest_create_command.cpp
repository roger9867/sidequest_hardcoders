//
// Created by roger on 6/30/25.
//
#include "quest_create_command.h"

#include <string>

#include "model/server_quest.h"
#include "network/json_serializable.h"
#include "storage/database.h"

namespace Sidequest::Server {

    QuestCreateCommand::QuestCreateCommand(Database* database)
    : database(database) {}

    void QuestCreateCommand::execute(const httplib::Request& request, httplib::Response& response) {

        set_cors_headers(response);

        std::cout << "calling QuestCreateCommand" << std::endl;
        std::cout << request.body << std::endl;
        auto json = Json::parse(request.body);
        std::cout << json << std::endl;
        auto quest = new ServerQuest(database, 0);
        quest->from_json(json);

        try {
            quest->create_on_database();
        }
        catch (UnableToCreateObjectException& e) {
            response.set_content(Json("unable to create quest"), "text/plain");
            response.status = httplib::StatusCode::BadRequest_400;
            std::cout << "unable to create user" << std::endl;
            return;
        }

        response.set_content(Json({"id", quest->get_id() } ).dump(), "text/plain");
        response.status = httplib::StatusCode::OK_200;
        std::cout << "quest created successfully" << std::endl;
    }

    std::string QuestCreateCommand::endpoint() {
        return "/api/quest/create";
    }

}  // namespace Sidequest::Server