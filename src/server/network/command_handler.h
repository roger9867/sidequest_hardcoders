//
// Created by roger on 6/30/25.
//

#ifndef COMMAND_HANDLER_H

#include <string>

#include "httplib.h"

namespace Sidequest::Server {

    typedef httplib::Request Request;
    typedef httplib::Response Response;

    class CommandHandler {
     public:
        virtual void execute(const Request& request, Response& response) = 0;
        virtual std::string endpoint() = 0;

        // execute umbauen, um spezifische Funktion zu erhalten
        std::function<void(const Request&, Response&)> get_function();
    };

}  // Namespace Sidequest::Server

#define COMMAND_HANDLER_H
#endif //COMMAND_HANDLER_H
