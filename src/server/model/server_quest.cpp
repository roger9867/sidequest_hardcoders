//
// Created by roger on 5/17/25.
//

#include "server_quest.h"

#include <iostream>
#include <ostream>

namespace Sidequest::Server {

    ServerQuest::ServerQuest(Database* database)
        : Persistable(database), SerializableQuest("no_id") {
    }

    ServerQuest::ServerQuest(Database* database, Id id)
        : Persistable(database)
        , Quest(id)
        , SerializableQuest(id) {}

    ServerQuest::ServerQuest(
        Database* database,
        Id id,
        Status status,
        std::string title,
        std::string caption,
        User* owner,
        User* editor,
        Quest *parent,
        std::vector<Quest *> subquests)
        : Persistable(database)
        , SerializableQuest(id, status, title, caption, owner, editor, parent)
        , Quest(id, status, title, caption,  owner, editor, parent, subquests){}

    ServerQuest::~ServerQuest() = default;

    void ServerQuest::create_on_database() {
        auto server_user_owner = new ServerUser(database, owner->get_email(), owner->get_display_name(), owner->get_password());
        try {
            server_user_owner->create_on_database();
        }
        catch (UnableToCreateObjectException ignored) {}

        auto server_user_editor = new ServerUser(database, editor->get_email(), editor->get_display_name(), editor->get_password());
        try {
            server_user_editor->create_on_database();
        }
        catch (UnableToCreateObjectException ignored) {}


        Query query = Query(database,"INSERT INTO quest (id, status, title, caption, owner_id, editor_id, parent_id) VALUES (?, ?, ?, ?, ?, ?, ?);");
        query.reset_statement();
        query.bind(1, id);
        query.bind(2, status_to_string(status));
        query.bind(3, title);
        query.bind(4, caption);

        if (owner) {
            query.bind(5, owner->get_email());
        } else {
            throw NoOwnerException("No owner");
        }
        if (editor) {
            query.bind(6, editor->get_email());
        } else {
            throw NoOwnerException("No editor");
        }
        if (parent) {
            query.bind(7, parent->get_id());
        } else {
            query.bind_null(7);
        }
        int result = query.execute();
        if (result != SQLITE_DONE) {
            std::cerr << "Failed to insert quest with id: " << id << std::endl;
            std::cerr << "SQLite error: " << sqlite3_errmsg(database->get_handle()) << std::endl;
            throw UnableToCreateObjectException("id");
        }

        delete(server_user_owner);
        query.reset_statement();
    }

    void ServerQuest::read_on_database()
    {
        Query query = Query(database,"SELECT * FROM quest WHERE id = ?;");
        query.reset_statement();
        query.bind(1, id);
        if (query.execute() != SQLITE_ROW)
            throw UnableToReadObjectException("id");

        status = string_to_status(query.read_text_value("status"));
        title = query.read_text_value("title");
        caption = query.read_text_value("caption");
        owner = new ServerUser(database, query.read_text_value("owner_id"));

        if (query.read_text_value("editor_id") != owner->get_email()) {
            editor = new ServerUser(database, query.read_text_value("editor_id"));
        } else {
            editor = owner;
        }

        if (query.read_text_value("parent_id") != "" && query.read_text_value("parent_id") != "null")
        {
            parent = new ServerQuest(database, query.read_text_value("parent_id"));
        }

        // Get one level of subquests
        // load_sub_quests();
        query.reset_statement();
    }


    void ServerQuest::update_on_database() {
        Query query = Query(database,"UPDATE quest SET status = ?, title = ?, caption = ?, owner_id = ?, editor_id = ?,  parent_id = ? WHERE id = ?;");
        query.reset_statement();
        query.bind(1, status_to_string(status));
        query.bind(2, title);
        query.bind(3, caption);
        query.bind(4, owner->get_email());
        query.bind(5, editor->get_email());

        if (parent) {
            query.bind(6, parent->get_id());
        } else {
            query.bind_null(6);
        }
        query.bind(7, id);

        if (query.execute() != SQLITE_DONE)
            throw UnableToUpdateObjectException("id");

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

    void ServerQuest::load_sub_quests() {
        auto query = Query(database, "SELECT * FROM quest WHERE parent_id=?;");

        try {
            query.bind(1, id);
        } catch (UnableToCreateObjectException& e)
        {
            std::cerr << "Binding failed: " << e.what() << std::endl;

        }

        int status = query.execute();
        if (status != SQLITE_ROW && status != SQLITE_DONE)
        {
            throw UnableToReadObjectException("id");
        }

        if (status == SQLITE_ROW) {
            for (auto it = query.begin(); it != query.end(); ++it) {
                auto row = *it;

                auto subquest =
                    new ServerQuest(
                        database,
                        row["id"],
                        string_to_status(row["status"]),
                        "title",
                     row["caption"],
                        nullptr,
                        nullptr,
                        this,
                        std::vector<Quest*>()
                );
                subquest->read_on_database();
                subquests.push_back(subquest);
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


    /*
    std::vector<ServerQuest> get_quests_by_parent(Id quest_id) {

        auto parent_quest = new ServerQuest(database,quest_id);

    }*/


    std::string ServerQuest::class_id() {
        return "quest";
    }
}
