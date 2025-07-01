//
// Created by roger on 6/30/25.
//
#include "json_serializable.h"
#include "model/id.h"

namespace Sidequest {

    // falls der pointer keinen wert hat, null json zurückgeben
    Json JsonSerializable::serialize_loadable_pointer(JsonSerializable* ptr, std::optional<Id> id, SerializationMode mode, int depth) {
        if (!id.has_value())
            return nullptr; // JSON null

        if (ptr == nullptr)
            return nullptr; // JSON null

        // falls nur das Objekt selbst geladen werden soll, 1 objekt erstellen
        if (mode == SerializationMode::no_level) {
            // Erstelle ein Objekt mit nur der ID drin statt nur String
            return Json::object({{"id", id.value()}});
        }

        // falls nicht no_level, ptr to json, erstmal explizit nur all_levels
        if (mode == SerializationMode::one_level)
            return ptr->to_json(one_level, depth + 1);

        if (mode == SerializationMode::all_levels)
            return ptr->to_json(all_levels);
    }



}  // namespace Sidequest
