//
// Created by roger on 5/17/25.
//

#include "server_quest.h"

#include <iostream>
#include <ostream>

#include "server_user.h"
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

    ServerQuest::ServerQuest(Database* database, Id id, User* owner, std::string caption, Quest *parent, std::vector<Quest *> subquests)
        : Persistable(database)
        , Quest(id, owner, caption, parent, subquests) {
    }

    ServerQuest::~ServerQuest() = default;




    void ServerQuest::create_on_database() {

        // was wenn Owner nicht gespeichert?

        try {
            auto s_user = new ServerUser(database, owner->get_email(), owner->get_display_name(), owner->get_password());
            s_user->create_on_database();
            owner = s_user;
        } catch (UnableToCreateObjectException ignored) {}


        Query query = Query(database,"INSERT INTO quest (id, owner_id, caption, parent_id) VALUES (?, ?, ?, ?);");
        query.bind(1, id);
        if (owner) {
            query.bind(2, owner->get_email());
        } else {
            throw NoOwnerException("No owner");
        }
        query.bind(3, caption);
        if (parent) {
            query.bind(4, parent->get_id());
        } else {
            query.bind(4, "null");
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
        owner = new ServerUser(database, query.read_text_value("owner_id"));

        if (query.read_text_value("parent_id") != "" && query.read_text_value("parent_id") != "null") {
            parent = new ServerQuest(database, query.read_text_value("parent_id"));
            std::cout << "has ----- parent" << std::endl;
            //parent->id = query.read_text_value("parent_id");
        }

        // Get one level of subquests
        this->get_subquests();
    }

    void ServerQuest::update_on_database() {
        Query query = Query(database,"UPDATE quest set owner_id=?, caption=?, parent_id=? WHERE id=?;");
        query.bind(1, owner->get_email());
        query.bind(2, caption);
        if (parent) {
            query.bind(3, parent->get_id());
        } else {
            query.bind(4, "null");
        }
        query.bind(4, id);
        if (query.execute() != SQLITE_DONE)
            throw UnableToUpdateObjectException("id");

        // update subquests ?
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

    void ServerQuest::get_subquests() {
        auto query = Query(database, "SELECT * FROM quest WHERE parent_id = ?;");
        query.bind(1, this->id);
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
                    new ServerQuest(
                        database,
                        row["id"],
                        owner,
                        row["caption"],
                        parent,
                        std::vector<Quest*>())
                );
                std::cout
                << row["id"]
                << ", " << row["owner_id"]
                << ", " << row["caption"]
                << ", " << row["parent_id"]
                << std::endl;
            }
        }
    }


    /* Main Quests haben keinen parent
     *
    void ServerQuest::get_parent() {

        auto query = Query(database, "SELECT * FROM quest q JOIN quest p on q.parent_id = p.id");
        query.bind(1, id);
        int status = query.execute();
        if (status != SQLITE_DONE && status != SQLITE_ROW)
            throw UnableToReadObjectException("Searching for parents failed");

        if (status == SQLITE_ROW) {
            auto it = query.begin();
            auto row = *it;
            this->parent = new ServerQuest(
                database,
                row["id"]);
            auto owner = (new ServerUser(   // read auf speziellen ServerUntertyp ausführen, an Obertyp zuweisen
                database,
                row["owner_id"]));
            owner->read_on_database();
            this->owner = owner;
        }
    }
    */


    std::string ServerQuest::class_id() {
        return "quest";
    }



}
