//
// Created by roger on 5/17/25.
//

#ifndef SERVER_QUEST_H
#define SERVER_QUEST_H
#include "model/quest.h"
#include "storage/persistable.h"


namespace Sidequest::Server {

    class ServerQuest : public Sidequest::Quest, public Sidequest::Server::Persistable {
    public:
        typedef std::string Id;

        explicit ServerQuest(Database* database);
        ServerQuest(Database* database, Id id);
        ServerQuest(Database* database, Id id, User* owner, std::string caption, Quest* parent, std::vector<Quest*> subquests);
        ~ServerQuest();

        void get_subquests();

        void create_on_database() override;
        void read_on_database() override;
        void update_on_database() override;
        void delete_on_database() override;

        std::string class_id() override;

        //void get_parent();
    };
}


#endif //SERVER_QUEST_H
