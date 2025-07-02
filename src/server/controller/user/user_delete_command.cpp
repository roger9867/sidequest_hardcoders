//
// Created by roger on 6/30/25.
//
#include "user_delete_command.h"
#include "user_delete_command.h"

#include <string>

#include "model/server_user.h"
#include "storage/database.h"

namespace Sidequest::Server {

    UserDeleteCommand::UserDeleteCommand(Database* database)
    : database(database) {}

    void UserDeleteCommand::execute(const httplib::Request& request, httplib::Response& response) {
        std::cout << "Calling UserDeleteCommand." << std::endl;
        Id user_id = request.path_params.at("id");
        auto user = new ServerUser(database, user_id);

        try {
            user->delete_on_database();
        }
        catch (UnableToCreateObjectException& e) {
            response.set_content(Json("Unable to delete user."), "text/plain");
            response.status = httplib::StatusCode::BadRequest_400;
            std::cout << "unable to delete user" << std::endl;
            return;
        }

        response.set_content("", "text/plain");
        response.status = httplib::StatusCode::OK_200;
        std::cout << "User deleted successfully." << std::endl;
    }

    std::string UserDeleteCommand::endpoint() {
        return "/api/user/:id/delete";
    }

}  // namespace Sidequest::Server