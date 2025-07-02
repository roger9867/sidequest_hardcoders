//
// Created by roger on 6/30/25.
//

#ifndef NETWORK_HANDLER_H

#include <string>
#include <unordered_map>
#include <httplib.h>


namespace httplib {
    class Server;
}

namespace Sidequest::Server {

    class CommandHandler;

    class ConnectionHandler {
     public:

        ConnectionHandler(std::string hostname, int port);
        ~ConnectionHandler();

        void register_get_command(CommandHandler* handler);
        void register_put_command(CommandHandler* handler);
        void register_delete_command(CommandHandler* handler);
        void register_patch_command(CommandHandler* handler);
        void register_post_command(CommandHandler* handler);

        void register_options_handler(std::function<void(const httplib::Request&, httplib::Response&)> handler);
        void listen();

     protected:
        std::unordered_map<std::string, CommandHandler*> endpoints;
        httplib::Server* server;
        std::string hostname;
        int port;
    };

} // namespace Sidequest::Server

#define NETWORK_HANDLER_H
#endif //NETWORK_HANDLER_H
