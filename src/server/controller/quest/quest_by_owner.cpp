//
// Created by roger on 7/1/25.
//
#include "quest_by_owner.h"

#include <string>

#include "model/server_quest.h"
#include "network/json_serializable.h"
#include "storage/database.h"

namespace Sidequest::Server {

    QuestByOwnerCommand::QuestByOwnerCommand(Database* database)
    : database(database) {}

    void QuestByOwnerCommand::execute(const httplib::Request& request, httplib::Response& response) {

        set_cors_headers(response);

        std::cout << "Calling QuestByParentCommand" << std::endl;
        Id owner_id = request.path_params.at("id");
        auto owner = new ServerUser(database, owner_id);

        try {
            owner->load_quests_by_owner();
        }
        catch (std::exception& e) {
            response.set_content(Json("Quests by owner failed"), "text/plain");
            response.status = httplib::StatusCode::BadRequest_400;
            return;
        }
        if (((User*)owner)->get_main_quests().size() == 0) {
            response.set_content(Json("No quests could be found."), "text/plain");
            response.status = httplib::StatusCode::NotFound_404;
            return;
        }

        auto quests_by_owner = owner->get_main_quests();
        nlohmann::json j_quests_by_owner = nlohmann::json::array();

        for (auto owned_quest : quests_by_owner) {

            auto serializable_quest = SerializableQuest(
                owned_quest->get_id(),
                owned_quest->get_status(),
                owned_quest->get_title(),
                owned_quest->get_caption(),
                owner,
                owned_quest->get_editor(),
                owned_quest->get_parent()
                );

                j_quests_by_owner.push_back(serializable_quest.to_json());
        }

        std::string quests_as_json_array = j_quests_by_owner.dump();

        response.set_content(quests_as_json_array, "text/plain");
        response.status = httplib::StatusCode::OK_200;

    }

    std::string QuestByOwnerCommand::endpoint() {
        return "/api/quest/:id/by_owner";
    }

}  // namespace Sidequest::Server