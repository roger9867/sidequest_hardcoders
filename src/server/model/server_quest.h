//
// Created by roger on 5/17/25.
//

#ifndef SERVER_QUEST_H
#define SERVER_QUEST_H
#include "model/quest.h"
#include "../sidequest/model/id.h"
#include "storage/persistable.h"
#include "server_user.h"
#include "network/serializable_quest.h"
#include "network/serializable_user.h"
#include "storage/database_exceptions.h"
#include "storage/Query.h"


namespace Sidequest::Server {

    class ServerQuest : public virtual Sidequest::Quest,
        public Sidequest::Server::Persistable,
        public SerializableQuest {
    public:

        explicit ServerQuest(Database* database);
        ServerQuest(Database* database, Id id);
        ServerQuest(
            Database* database,
            Id id,
            Status status,
            std::string title,
            std::string caption,
            User* owner,
            User* editor,
            Quest* parent,
            std::vector<Quest*> subquests);

        ~ServerQuest();

        void load_sub_quests();

        void create_on_database() override;
        void read_on_database() override;
        void update_on_database() override;
        void delete_on_database() override;

        std::string class_id() override;

    };
}


#endif //SERVER_QUEST_H
