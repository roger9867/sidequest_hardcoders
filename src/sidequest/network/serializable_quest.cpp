//
// Created by roger on 6/30/25.
//
#include "serializable_quest.h"

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

    Json SerializableQuest::to_json( bool full_serialise ) {
        if (!full_serialise)
            return Json(id);

        Json json = {
            { "id", id },
            { "title", title },
            { "caption", caption },
            // filesystem::status
            { "status", status },
        };

        json["parent"] = serialize_loadable_pointer(dynamic_cast<JsonSerializable*>(parent), parent->get_id(), serialize_parent_recursive);
        json["owner"]  = serialize_loadable_pointer(dynamic_cast<JsonSerializable*>(owner),  owner->get_email(),  serialize_owner_recursive);
        json["editor"] = serialize_loadable_pointer(dynamic_cast<JsonSerializable*>(editor), editor->get_email(), serialize_editor_recursive);

        /*json["subquests"] = Json::array();
        for (auto subquest : subquests)
        {
            auto _subquest = dynamic_cast<JsonSerialisable*>(subquest);
            json["subquests"].push_back( _subquest->to_json(!serialise_subquests_recursive) );
        }*/

        return json;
    }

    void SerializableQuest::from_json(const Json& json) {
        // id might not exist of the user data is send as a create-user request,
        // such the id is not yet assigned
        if (json.contains("id"))
            json.at("id").get_to(id);

        title = json.at("title");
        caption = json.at("description");

        Status status = json.at("status");
        //std::filesystem::status = json.at("status");

        //neu anlegen:
        std::optional<Id> parent_id = parent->get_id();
        std::optional<Id> owner_id = editor->get_email();
        std::optional<Id> editor_id = editor->get_email();

        parent = deserialize_loadable_pointer<SerializableQuest>(json.at("parent"), parent_id);
        owner  = deserialize_loadable_pointer<SerializableUser> (json.at("owner"),  owner_id);
        editor = deserialize_loadable_pointer<SerializableUser> (json.at("editor"), editor_id);
    }

}