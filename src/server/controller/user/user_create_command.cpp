//
// Created by roger on 6/30/25.
//
#include "user_create_command.h"
#include "user_create_command.h"

#include <string>

#include "model/server_user.h"
#include "storage/database.h"

namespace Sidequest::Server {

    UserCreateCommand::UserCreateCommand(Database* database)
    : database(database) {}

    void UserCreateCommand::execute(const httplib::Request& request, httplib::Response& response) {
        std::cout << "calling UserCreateCommand" << std::endl;
        std::cout << request.body << std::endl;
        auto json = Json::parse(request.body);
        std::cout << json << std::endl;
        auto user = new ServerUser(database, 0);
        user->from_json(json);

        try {
            user->create_on_database();
        }
        catch (UnableToCreateObjectException& e) {
            response.set_content(Json("unable to create user"), "text/plain");
            response.status = httplib::StatusCode::BadRequest_400;
            std::cout << "unable to create user" << std::endl;
            return;
        }

        response.set_content(Json({"id", user->get_id() }).dump(), "text/plain");
        response.status = httplib::StatusCode::OK_200;
        std::cout << "user created successfully" << std::endl;
    }

    std::string UserCreateCommand::endpoint() {
        return "/api/user/create";
    }

}  // namespace Sidequest::Server