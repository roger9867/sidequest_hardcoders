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

    Json SerializableUser::to_json(SerializationMode mode, int depth) {

        if (mode == no_level)
            return Json(email);

        Json json = {
            { "email", email },
            { "display_name", display_name }
        };

        return json;
    }

    void SerializableUser::from_json(const Json& json) {
        if (json.contains("email") && !json.at("email").is_null()) {
            json.at("email").get_to(email);
        } else if (json.contains("id") && !json.at("id").is_null()) {
            json.at("id").get_to(email);
        } else {
            email = "";
        }

        if (json.contains("display_name") && !json.at("display_name").is_null()) {
            json.at("display_name").get_to(display_name);
        } else {
            display_name = "";
        }
    }
}