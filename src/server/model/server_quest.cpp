//
// Created by roger on 5/17/25.
//

#include "server_quest.h"

#include <iostream>
#include <ostream>

namespace Sidequest::Server {

    ServerQuest::ServerQuest(Database* database)
        : Persistable(database)
    {
    }

    ServerQuest::ServerQuest(Database* database, Id id)
        : Persistable(database)
        , Quest(id)
    {
    }

    ServerQuest::ServerQuest(Database* database, Id id, User* owner, std::string caption, Quest *parent, std::vector<Quest *> subquests)
        : Persistable(database)
        , Quest(id, owner, caption, parent, subquests)
    {
    }

    ServerQuest::~ServerQuest() = default;


    void ServerQuest::create_on_database()
    {
        auto server_user_owner = new ServerUser(database, owner->get_email(), owner->get_display_name(), owner->get_password());
        // save owner if not in database only
        try {
            server_user_owner->create_on_database();
        }
        catch (UnableToCreateObjectException ignored)
        {
        }

        Query query = Query(database,"INSERT INTO quest (id, owner_id, caption, parent_id) VALUES (?, ?, ?, ?);");
        query.reset_statement();
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
            query.bind_null(4);
        }
        int result = query.execute();
        if (result != SQLITE_DONE) {
            std::cerr << "Failed to insert quest with id: " << id << std::endl;
            std::cerr << "SQLite error: " << sqlite3_errmsg(database->get_handle()) << std::endl;
            throw UnableToCreateObjectException("id");
        }

        delete(server_user_owner);
        query.reset_statement();
        std::cout << "[INSERT QUEST] id: " << id << ", parent: "
          << (parent ? parent->get_id() : "null") << std::endl;

    }

    void ServerQuest::read_on_database()
    {
        Query query = Query(database,"SELECT * FROM quest WHERE id = ?;");
        query.reset_statement();
        query.bind(1, id);
        if (query.execute() != SQLITE_ROW)
            throw UnableToReadObjectException("id");
        caption = query.read_text_value("caption");
        owner = new ServerUser(database, query.read_text_value("owner_id"));

        if (query.read_text_value("parent_id") != "" && query.read_text_value("parent_id") != "null")
        {
            parent = new ServerQuest(database, query.read_text_value("parent_id"));
        }

        // Get one level of subquests
        std::cout << "here " << std::endl;
        //load_sub_quests();
        query.reset_statement();
    }

    void ServerQuest::update_on_database() {
        Query query = Query(database,"UPDATE quest SET owner_id = ?, caption = ?, parent_id = ? WHERE id = ?;");
        query.reset_statement();
        query.bind(1, owner->get_email());
        query.bind(2, caption);
        if (parent) {
            query.bind(3, parent->get_id());
        } else {
            query.bind_null(3);  // Use bind_null instead of a string "null"
        }
        query.bind(4, id);

        if (query.execute() != SQLITE_DONE)
            throw UnableToUpdateObjectException("id");

        // update subquests ?
        query.reset_statement();
    }

    void ServerQuest::delete_on_database() {
        Query query = Query(database,"DELETE FROM quest WHERE id=?;");
        query.reset_statement();
        query.bind(1, id);
        if (query.execute() != SQLITE_DONE)
            throw UnableToDeleteObjectException("id");
        query.reset_statement();
    }


    void ServerQuest::load_sub_quests()
    {
        auto query = Query(database, "SELECT * FROM quest WHERE parent_id=?;");

        std::cout << "load subquests" << std::endl;
        //std::cout << "Parameter_ID = " << id << std::endl;
        try {
            query.bind(1, id);
        } catch (UnableToCreateObjectException& e)
        {
                std::cerr << "Binding failed: " << e.what() << std::endl;

        }
        //this->subquests.clear();

        int status = query.execute();
        if (status != SQLITE_ROW && status != SQLITE_DONE)
        {
            throw UnableToReadObjectException("id");
        }

        if (status == SQLITE_DONE)
            std::cout << "~ " << status << std::endl;

        if (status == SQLITE_ROW) {
            std::cout << "has_row : " << id << std::endl;
            for (auto it = query.begin(); it != query.end(); ++it) {
                auto row = *it;

                auto subquest =
                    new ServerQuest(
                        database,
                        row["id"],
                        owner,
                     row["caption"],
                        this,
                        std::vector<Quest*>()
                );
                subquest->read_on_database();
                std::cout << "read" << std::endl;
                subquests.push_back(subquest);
                //std::cout << "%%11%% " << subquest->get_id() << std::endl;
            }
        }

        query.reset_statement();
    }



    /* main quests dont have a parent
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
