//
// Created by roger on 6/30/25.
//
#include "user_update_command.h"

#include <string>

#include "model/server_user.h"
#include "storage/database.h"

namespace Sidequest::Server {

    UserUpdateCommand::UserUpdateCommand(Database* database)
    : database(database) {}

    void UserUpdateCommand::execute(const httplib::Request& request, httplib::Response& response) {
        std::cout << "calling UserUpdateCommand" << std::endl;
        std::cout << request.body << std::endl;
        auto json = Json::parse(request.body);
        std::cout << json << std::endl;
        auto user = new ServerUser(database, 0);
        user->from_json(json);

        try {
            user->update_on_database();
        }
        catch (UnableToCreateObjectException& e) {
            response.set_content(Json("unable to update user"), "text/plain");
            response.status = httplib::StatusCode::BadRequest_400;
            std::cout << "unable to update user" << std::endl;
            return;
        }

        response.set_content(Json().dump(), "text/plain");
        response.status = httplib::StatusCode::OK_200;
        std::cout << "user updated successfully" << std::endl;
    }

    std::string UserUpdateCommand::endpoint() {
        return "/api/user/:id/update";
    }

}  // namespace Sidequest::Server