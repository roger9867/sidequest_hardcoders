//
// Created by roger on 6/30/25.
//
#include "quest_read_command.h"

#include <string>

#include "quest_read_command.h"
#include "model/server_quest.h"
#include "network/json_serializable.h"
#include "storage/database.h"

namespace Sidequest::Server {

    QuestReadCommand::QuestReadCommand(Database* database)
        : database(database) {}

    void QuestReadCommand::execute(const httplib::Request& request, httplib::Response& response) {

        set_cors_headers(response);

        std::cout << "Calling QuestReadCommand." << std::endl;
        Id user_id = request.path_params.at("id");
        auto user = new ServerQuest(database, user_id);

        try {
            user->read_on_database();
        }
        catch (UnableToReadObjectException& e) {
            response.set_content(Json("No such quest."), "text/plain");
            response.status = httplib::StatusCode::NotFound_404;
            return;
        }

        std::string user_as_json = user->to_json().dump();
        response.set_content(user_as_json, "text/plain");
        response.status = httplib::StatusCode::OK_200;
    }

    std::string QuestReadCommand::endpoint() {
        return "/api/quest/:id/read";
    }

}  // namespace Sidequest::Server