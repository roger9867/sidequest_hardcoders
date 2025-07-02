//
// Created by roger on 6/30/25.
//
#include "serializable_quest.h"

#include <iostream>
#include <stdexcept>

#include "serializable_user.h"
#include "model/user.h"
#include "serializable_user.h"

namespace Sidequest {

    SerializableQuest::SerializableQuest(Id id)
        : Quest(id) {}

    SerializableQuest::SerializableQuest(
        Id id,
        Status status,
        std::string title,
        std::string caption,
        User* owner,
        User* editor,
        Quest* parent)
        : Quest(id, status, title, caption, owner, editor, parent) {}

    SerializableQuest::~SerializableQuest() {}

    Json SerializableQuest::to_json(SerializationMode mode, int depth) {
    if (mode == no_level)
        return Json::object({ {"id", id} });

    Json json = {
        { "id", id },
        { "status", status_to_string(status) },
        { "title", title },
        { "caption", caption }
    };

    if (owner != nullptr) {
        if (mode == one_level && depth > 0) {
            json["owner"] = Json::object({ { "email", owner->get_email() } });
        } else {
            json["owner"] = serialize_loadable_pointer(
                dynamic_cast<JsonSerializable*>(owner),
                owner->get_email(),
                mode,
                depth + 1
            );
        }
    } else {
        json["owner"] = nullptr;
    }

    if (editor != nullptr) {
        if (mode == one_level && depth > 0) {
            json["editor"] = Json::object({ { "email", editor->get_email() } });
        } else {
            json["editor"] = serialize_loadable_pointer(
                dynamic_cast<JsonSerializable*>(editor),
                editor->get_email(),
                mode,
                depth + 1
            );
        }
    } else {
        json["editor"] = nullptr;
    }

    if (parent != nullptr) {
        if (mode == one_level && depth > 0) {
            json["parent"] = Json::object({ { "id", parent->get_id() } });
        } else {
            json["parent"] = serialize_loadable_pointer(
                dynamic_cast<JsonSerializable*>(parent),
                parent->get_id(),
                mode,
                depth + 1
            );
        }
    } else {
        json["parent"] = nullptr;
    }

    json["subquests"] = Json::array();
    for (auto subquest : subquests) {
        if (!subquest)
            continue;

        auto _subquest = dynamic_cast<JsonSerializable*>(subquest);
        if (_subquest)
            json["subquests"].push_back(_subquest->to_json(no_level)); // shallow ref
    }

    return json;
}


    void SerializableQuest::from_json(const Json& json) {
    if (json.contains("id") && !json.at("id").is_null())
        json.at("id").get_to(id);

    if (json.contains("title") && !json.at("title").is_null())
        json.at("title").get_to(title);

    if (json.contains("caption") && !json.at("caption").is_null())
        json.at("caption").get_to(caption);

    if (json.contains("status") && !json.at("status").is_null()) {
        std::string status_str;
        json.at("status").get_to(status_str);
        status = Quest::string_to_status(status_str);
    }

    parent = nullptr;
    std::optional<Id> parent_id;
    if (json.contains("parent") && !json.at("parent").is_null()) {
        const auto& parent_json = json.at("parent");
        if (parent_json.is_object() && parent_json.contains("id") && !parent_json.at("id").is_null())
            parent_id = parent_json.at("id").get<std::string>();
        parent = deserialize_loadable_pointer<SerializableQuest>(parent_json, parent_id);
    }

    owner = nullptr;
    std::optional<Id> owner_id;
    if (json.contains("owner") && !json.at("owner").is_null()) {
        const auto& owner_json = json.at("owner");
        if (owner_json.is_object() && owner_json.contains("email") && !owner_json.at("email").is_null())
            owner_id = owner_json.at("email").get<std::string>();
        owner = deserialize_loadable_pointer<SerializableUser>(owner_json, owner_id);
    }

    editor = nullptr;
    std::optional<Id> editor_id;
    if (json.contains("editor") && !json.at("editor").is_null()) {
        const auto& editor_json = json.at("editor");
        if (editor_json.is_object() && editor_json.contains("email") && !editor_json.at("email").is_null())
            editor_id = editor_json.at("email").get<std::string>();
        editor = deserialize_loadable_pointer<SerializableUser>(editor_json, editor_id);
    }

    subquests.clear();
    if (json.contains("subquests") && json.at("subquests").is_array()) {
        for (const auto& subquest_json : json.at("subquests")) {
            std::optional<Id> subquest_id;
            if (subquest_json.contains("id") && !subquest_json.at("id").is_null())
                subquest_id = subquest_json.at("id").get<std::string>();

            auto subquest = deserialize_loadable_pointer<SerializableQuest>(subquest_json, subquest_id);
            if (subquest != nullptr)
                subquests.push_back(subquest);
        }
    }
}





}