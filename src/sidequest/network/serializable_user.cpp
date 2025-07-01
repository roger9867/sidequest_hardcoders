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
        // nur 1 Objekt falls nicht all_levels
        if (mode != all_levels)
            return Json(email);

        Json json = {
            { "email", email },
            { "display_name", display_name }
            //{ "email", email }
        };

        return json;
    }

    void SerializableUser::from_json(const Json& json) {
        if (json.contains("email") && !json.at("email").is_null()) {
            json.at("email").get_to(email);
        } else if (json.contains("id") && !json.at("id").is_null()) {
            json.at("id").get_to(email);
        } else {
            email = ""; // oder throw std::runtime_error("missing email or id");
        }

        if (json.contains("display_name") && !json.at("display_name").is_null()) {
            json.at("display_name").get_to(display_name);
        } else {
            display_name = "";
        }
    }
}