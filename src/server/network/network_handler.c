//
// Created by roger on 6/30/25.
//

#include "connection_handler.h"
#include "httplib.h"
#include "network/command_handler.h"

namespace Sidequest::Server {

        ConnectionHandler::ConnectionHandler(std::string hostname, int port)
            : hostname(hostname), port(port) {
			// neues http-Server-Objekt erstellen
            server = new httplib::Server();
        }

        ConnectionHandler::~ConnectionHandler() {
            // Alle Commandhandler für die Strings freigeben
            for (auto& iter : endpoints)
                delete(iter.second);
        }

        void ConnectionHandler::register_get_command(CommandHandler* handler) {
            auto function = handler->get_function();
            server->GET(handler->endpoint(), function);
        }

        void ConnectionHandler::register_put_command(CommandHandler* handler) {
            auto function = command->get_function();
            server->PUT(command->endpoint(), function);
        }

        void ConnectionHandler::listen() {
            server->listen(hostname, port);
        }
 } // namespace Server
