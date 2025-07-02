//
// Created by roger on 6/30/25.
//

#include "network_handler.h"
#include "httplib.h"
#include "command_handler.h"

namespace Sidequest::Server {

        ConnectionHandler::ConnectionHandler(std::string hostname, int port)
            : hostname(hostname), port(port) {
            server = new httplib::Server();
        }

        ConnectionHandler::~ConnectionHandler() {
            for (auto& iter : endpoints)
                delete(iter.second);
        }

        void ConnectionHandler::register_get_command(CommandHandler* handler) {
            auto function = handler->get_function();
            server->Get(handler->endpoint(), function);
        }

        void ConnectionHandler::register_put_command(CommandHandler* handler) {
            auto function = handler->get_function();
            server->Put(handler->endpoint(), function);
        }

        void ConnectionHandler::register_delete_command(CommandHandler* handler) {
            auto function = handler->get_function();
            server->Delete(handler->endpoint(), function);
        }

        void ConnectionHandler::register_post_command(CommandHandler* handler) {
            auto function = handler->get_function();
            server->Post(handler->endpoint(), function);
        }

         void ConnectionHandler::register_patch_command(CommandHandler* handler) {
            auto function = handler->get_function();
            server->Patch(handler->endpoint(), function);
        }

        void ConnectionHandler::listen() {
            server->listen(hostname, port);
        }

    void ConnectionHandler::register_options_handler(std::function<void(const httplib::Request&, httplib::Response&)> handler) {
            server->Options(".*", [](const httplib::Request& req, httplib::Response& res) {
            std::cout << "OPTIONS request: " << req.path << std::endl;

    res.set_header("Access-Control-Allow-Origin", "http://localhost:3001");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
    res.status = 204;
});

        }
 } // namespace Server
