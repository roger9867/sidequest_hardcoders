//
// Created by roger on 6/30/25.
//
#include "json_serializable.h"
#include "model/id.h"

namespace Sidequest {

    Json JsonSerializable::serialize_loadable_pointer(JsonSerializable* ptr, std::optional<Id> id, bool full_serialise) {
        if (!id.has_value())
            return Json();

        if (!full_serialise)
            return Json(id.value());

        if (ptr == nullptr)
            return Json(id.value());

        return ptr->to_json();
    }

}  // namespace Sidequest
