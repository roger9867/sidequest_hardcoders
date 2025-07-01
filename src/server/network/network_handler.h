//
// Created by roger on 6/30/25.
//

#ifndef NETWORK_HANDLER_H

#include <string>
#include <unordered_map>

namespace httplib {
    class Server;
}

namespace Sidequest::Server {
    // command handler bekannt machen
    class CommandHandler;

    class ConnectionHandler {
     public:
        // wird mit hostname u. port erstellt
        ConnectionHandler(std::string hostname, int port);
        ~ConnectionHandler();

        // einen Funktionshandler als GET registrieren
        void register_get_command(CommandHandler* handler);
        void register_put_command(CommandHandler* handler);

        // Anfangen, einen angebenen port abzuhören
        void listen();

     protected:
        // Networkhandler ordned einem string einen generische Funktion zu
        std::unordered_map<std::string, CommandHandler*> endpoints;
        // hat http-Serverobjekt
        httplib::Server* server;
        // host hat selbst einen Namen
        std::string hostname;
        // port, den der Server abhört
        int port;
    };

} // namespace Sidequest::Server

#define NETWORK_HANDLER_H
#endif //NETWORK_HANDLER_H
