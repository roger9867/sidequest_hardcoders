//
// Created by roger on 6/30/25.
//

#ifndef JSON_SERIALISABLE_H
#include <optional>
#include <json.hpp>

#include "model/id.h"

typedef nlohmann::json Json;

namespace Sidequest {

    /* All classes that can be transferred via network should implement this interface */
    class JsonSerializable {
     public:
        enum SerializationMode {
            no_level,
            one_level,
            all_levels
        };

        //to json hat jetzt 3 modi
        virtual Json to_json(SerializationMode = no_level, int depth = 0) = 0;
        virtual void  from_json(const Json& json) = 0;

        // auch 3 modi, default wurde von alle auf 0 geändert
        Json serialize_loadable_pointer(JsonSerializable* ptr, std::optional<Id> id, SerializationMode mode = no_level, int depth = 0);

        // braucht keine pointer
        template<class T>
        T* deserialize_loadable_pointer(const Json& json, std::optional<Id>& id) {
            // falls der json null, kommt nullptr zurück
            if (json.is_null()) {
                id = std::nullopt;
                return nullptr;
            }
            // ???
            if (json.is_number_unsigned()) {
                Id _id;
                json.get_to(_id);
                id = _id;
                return nullptr;
            }
            // Ist der Json ein objekt statt liste
            if (json.is_object()) {
                T* object = new T("0");
                object->from_json(json);
                id = object->get_id();
                return object;
            }
            throw std::runtime_error("illegal json object");
        }
    };

}  // namespace Sidequest

#define JSON_SERIALISABLE_H
#endif //JSON_SERIALISABLE_H
