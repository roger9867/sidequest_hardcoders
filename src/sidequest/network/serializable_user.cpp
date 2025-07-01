//
// Created by roger on 6/30/25.
//


#include <stdlib.h>
#include <json.hpp>

#include "serializable_user.h"

using namespace nlohmann;

namespace Sidequest {

    SerializableUser::SerializableUser(Id email)
        : User(email) {}

    SerializableUser::SerializableUser(std::string email, std::string display_name, std::string password)
        : User( email, display_name, password ) {}

    SerializableUser::~SerializableUser() {}

    Json SerializableUser::to_json( bool full_serialise ) {
        if (!full_serialise)
            return Json(email);

        Json json = {
            { "id", email },
            { "display_name", display_name },
            //{ "email", email }
        };

        return json;
    }

    void SerializableUser::from_json(const Json& json) {
        // id might not exist of the user data is send as a create-user request,
        // such the id is not yet assigned
        if (json.contains("id"))
            json.at("id").get_to(email);
        json.at("display_name").get_to(display_name);
        //json.at("email").get_to(email);
    }
}