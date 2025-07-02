//
// Created by roger on 7/1/25.
//
#include "quest_by_parent.h"

#include <string>

#include "model/server_quest.h"
#include "network/json_serializable.h"
#include "storage/database.h"

namespace Sidequest::Server {

    QuestByParentCommand::QuestByParentCommand(Database* database)
    : database(database) {}

    void QuestByParentCommand::execute(const httplib::Request& request, httplib::Response& response) {

        set_cors_headers(response);

        std::cout << "Calling QuestByParentCommand" << std::endl;
        Id parent_id = request.path_params.at("id");
        auto parent = new ServerQuest(database, parent_id);

        try {
            parent->load_sub_quests();
        }
        catch (std::exception& e) {
            response.set_content(Json("Quests by parent failed"), "text/plain");
            response.status = httplib::StatusCode::BadRequest_400;
            return;
        }
        if (parent->get_subquests().size() == 0) {
            response.set_content(Json("No subquests could be found."), "text/plain");
            response.status = httplib::StatusCode::NotFound_404;
            return;
        }


        auto subquests = parent->get_subquests();
        nlohmann::json j_subquests = nlohmann::json::array();

        for (auto* subquest : subquests) {

            auto serializable_quest = SerializableQuest(
                subquest->get_id(),
                subquest->get_status(),
                subquest->get_title(),
                subquest->get_caption(),
                subquest->get_owner(),
                subquest->get_editor(),
                parent
                );

                j_subquests.push_back(serializable_quest.to_json());
        }

        std::string subquests_as_json_array = j_subquests.dump();

        response.set_content(subquests_as_json_array, "text/plain");
        response.status = httplib::StatusCode::OK_200;
    }

    std::string QuestByParentCommand::endpoint() {
        return "/api/quest/:id/by_parent";
    }

}  // namespace Sidequest::Server