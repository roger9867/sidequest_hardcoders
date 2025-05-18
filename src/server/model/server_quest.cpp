//
// Created by roger on 5/17/25.
//

#include "server_quest.h"

#include <iostream>
#include <ostream>

#include "storage/database_exceptions.h"
#include "storage/Query.h"


namespace Sidequest::Server {

    ServerQuest::ServerQuest(Database* database)
        : Persistable(database) {
    }

    ServerQuest::ServerQuest(Database* database, Id id)
        : Persistable(database)
        , Quest(id) {
    }

    ServerQuest::ServerQuest(Database* database, Id id, std::string caption, Quest *parent, std::vector<Quest *> *subquests)
        : Persistable(database)
        , Quest(id, caption, parent, subquests) {
    }

    ServerQuest::~ServerQuest() = default;




    void ServerQuest::create_on_database() {
        Query query = Query(database,"INSERT INTO quest (id, caption, parent_id) VALUES (?, ?, ?);");
        query.bind(1, id);

        query.bind(2, caption);
        if (parent) {
            query.bind(3, parent->id);
            std::cout << "##1" << std::endl;
        } else {
            query.bind(3, "null");
        }
        if (query.execute() != SQLITE_DONE)
            throw UnableToCreateObjectException("id");
    }

    void ServerQuest::read_on_database() {
        Query query = Query(database,"SELECT * FROM quest WHERE id = ?;");
        query.bind(1, id);
        if (query.execute() != SQLITE_ROW)
            throw UnableToReadObjectException("id");
        caption = query.read_text_value("caption");


        if (query.read_text_value("parent_id") != "") {
            parent = new ServerQuest(database, query.read_text_value("parent_id"));
            parent->id = query.read_text_value("parent_id");
        }

        subquests = ServerQuest::get_subquests(this);

    }

    void ServerQuest::update_on_database() {
        Query query = Query(database,"UPDATE quest set caption=?, parent=? WHERE id=?;");
        query.bind(1, caption);
        query.bind(2, parent->id);
        query.bind(3, id);
        if (query.execute() != SQLITE_DONE)
            throw UnableToUpdateObjectException("id");
    }

    void ServerQuest::delete_on_database() {
        Query query = Query(database,"DELETE FROM quest WHERE id=?;");
        query.bind(1, id);
        if (query.execute() != SQLITE_DONE)
            throw UnableToDeleteObjectException("id");
    }

    /*
    Quest* ServerQuest::get_parent(ServerQuest* quest) {
        auto query = Query(database, "SELECT * FROM quest WHERE parent_id = ?;");
    }
    */

    std::vector<Quest*> ServerQuest::get_subquests(ServerQuest* parent) {
        auto query = Query(database, "SELECT * FROM quest WHERE parent_id = ?;");
        query.bind(1, parent->id);
        auto sub_quests = std::vector<Quest*>();
        //std::cout << "$$" << std::endl;

        // Execute the query and check for errors
        int status = query.execute();
        if (status != SQLITE_ROW && status != SQLITE_DONE) {
            std::cout << "Error executing query" << std::endl;
            throw UnableToReadObjectException("id");
        }

        //std::cout << "$$---" << std::endl;
        // Iterate through query results
        // Only start iteration if there are results
        if (status == SQLITE_ROW) {
            for (auto it = query.begin(); it != query.end(); ++it) {
                auto row = *it;
                std::cout << "+++" << std::endl;
                sub_quests.push_back(
                    new ServerQuest(database, row["id"], row["caption"], parent, nullptr)
                );
                std::cout << row["id"] << ", " << row["caption"] << ", " << row["parent"] << std::endl;
            }
        }

        return sub_quests;
    }




    std::string ServerQuest::class_id() {
        return "quest";
    }

}
