//
// Created by roger on 6/30/25.
//

#include <iostream>
#include <json.hpp>

#include "connection_handler.h"
#include "httplib.h"
#include "command_handler.h"

using json = nlohmann::json;

namespace Sidequest::Server {

    std::function<void(const httplib::Request&, httplib::Response&)> CommandHandler::get_function() {
        // Neue Parameter auf Funktion binden u. somit neue erstellen
        auto function = std::bind(&CommandHandler::execute, this, std::placeholders::_1, std::placeholders::_2);
        return function;
    }

}  // namespace Sidequest::Server