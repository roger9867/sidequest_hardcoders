//
// Created by roger on 6/30/25.
//

#ifndef SERIALIZABLE_USER_H
#include <string>
#include <vector>

#include "model/id.h"
#include "model/user.h"
#include "network/json_serializable.h"

namespace Sidequest {

    class Quest;

    class SerializableUser : public virtual User, public JsonSerializable {
     public:
        SerializableUser(Id email);
        SerializableUser(Id email, std::string display_name, std::string password);
        virtual ~SerializableUser();

        // implementation of JsonSerialisable
        virtual Json to_json(bool full_serialise = true) override;
        virtual void from_json(const Json& json) override;
    };

}  // namespace Sidequest

#define SERIALIZABLE_USER_H
#endif //SERIALIZABLE_USER_H
