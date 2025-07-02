//
// Created by roger on 6/30/25.
//
#include <string>
#include <iostream>

#include "user_read_command.h"
#include "user_read_command.h"
#include "model/server_user.h"
#include "storage/database.h"

namespace Sidequest::Server {

        UserReadCommand::UserReadCommand(Database* database)
            : database(database) {}

        void UserReadCommand::execute(const httplib::Request& request, httplib::Response& response) {
            std::cout << "Calling UserReadCommand." << std::endl;
            Id user_id = request.path_params.at("id");
            auto user = new ServerUser(database, user_id);

            try {
                user->read_on_database();
            }
            catch (UnableToReadObjectException& e) {
                response.set_content(Json("No such user."), "text/plain");
                response.status = httplib::StatusCode::NotFound_404;
                return;
            }

            std::string user_as_json = user->to_json(JsonSerializable::one_level).dump();
            response.set_content(user_as_json, "text/plain");
            response.status = httplib::StatusCode::OK_200;
            std::cout << "User read correctly." << std::endl;
        }

        std::string UserReadCommand::endpoint() {
            return "/api/user/:id/read";
        }

} // namespace Sidequest::Server
