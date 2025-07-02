//
// Created by roger on 6/30/25.
//

#include <iostream>
#include <json.hpp>

#include "network_handler.h"
#include "httplib.h"
#include "command_handler.h"

using json = nlohmann::json;

namespace Sidequest::Server {

    std::function<void(const httplib::Request&, httplib::Response&)> CommandHandler::get_function() {
        auto function = std::bind(&CommandHandler::execute, this, std::placeholders::_1, std::placeholders::_2);
        return function;
    }

    void CommandHandler::set_cors_headers(httplib::Response& res, const std::string& origin ) {
        res.set_header("Access-Control-Allow-Origin", origin);
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
    }


}  // namespace Sidequest::Server