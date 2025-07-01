//
// Created by roger on 6/30/25.
//

#ifndef SERIALISABLE_QUEST_CPP_H
#include <string>
#include <vector>

#include "model/quest.h"
#include "network/json_serializable.h"

namespace Sidequest {

    class User;

    NLOHMANN_JSON_SERIALIZE_ENUM(Quest::Status,
        {
            { Quest::Status::initial, "initial" },
            { Quest::Status::done,    "done" },
            { Quest::Status::inactive, "inactive" }
        })

    class SerializableQuest : public virtual Quest, public JsonSerializable  {
     public:
        // Warum 1x mit Id u. einmal ohne?
        SerializableQuest(Id id);
        // Eig. keine Id hier, ergänzt ??!
        SerializableQuest(Id id, Status status, std::string title, std::string caption, User* owner, User* editor, Quest* parent);
        virtual ~SerializableQuest();

        // implementation of JsonSerialisable
        virtual Json to_json( bool full_serialise = true ) override;
        virtual void  from_json(const Json& json) override;

    public:
        bool serialize_parent_recursive = false;
        bool serialize_owner_recursive = false;
        bool serialize_editor_recursive = false;
        bool serialize_subquests_recursive = false;
    };

}  // namespace Sidequest

#define SERIALIZABLE_QUEST_CPP_H
#endif //SERIALIZABLE_QUEST_CPP_H
