#include "server_user.h"

#include "server_quest.h"
#include "../storage/database_exceptions.h"
#include "storage/database.h"

namespace Sidequest::Server {

	ServerUser::ServerUser(Database* database)
		: Persistable(database) {}
		
	ServerUser::ServerUser(Database* database, std::string email)
		: Persistable(database)
		, User(email){}

	ServerUser::ServerUser(Database* database, std::string email, std::string display_name, std::string password)
		: Persistable(database)
		, User(email, display_name, password) {}

	ServerUser::~ServerUser() = default;



	void ServerUser::create_on_database() {
		Query query = Query(database,"INSERT INTO user(email, display_name, password) VALUES (?, ?, ?);");
		query.bind(1, email);
		query.bind(2, display_name);
		query.bind(3, password);
		if (query.execute() != SQLITE_DONE)
			throw UnableToCreateObjectException(email);
	}


	void ServerUser::read_on_database() {
		Query query = Query(database,"SELECT * FROM user WHERE email = ?;");
		query.bind(1, email);
		if (query.execute() != SQLITE_ROW)
			throw UnableToReadObjectException(email);
		display_name = query.read_text_value("display_name");
		password     = query.read_text_value("password");
	}

	void ServerUser::update_on_database() {
		Query query = Query(database,"UPDATE user set display_name=?, password=? WHERE email=?;");
		query.bind(1, display_name);
		query.bind(2, password);
		query.bind(3, email);
		if (query.execute() != SQLITE_DONE)
			throw UnableToUpdateObjectException(display_name);
	}

	void ServerUser::delete_on_database() {
		Query query = Query(database,"DELETE FROM user WHERE email=?;");
		query.bind(1, email);
		if (query.execute() != SQLITE_DONE)
			throw UnableToDeleteObjectException(email);
	}

	std::string ServerUser::class_id() {
		return "user";
	}


	void ServerUser::load_main_quests() {
		auto query = Query(database, "SELECT * FROM quest WHERE owner_id = ? AND parent_id IS NULL;");
		query.bind(1, email);

		this->main_quests = std::vector<Quest*>();

		int status = query.execute();
		if (status != SQLITE_DONE && status != SQLITE_ROW)
			throw UnableToReadObjectException("Loading main quests failed");

		if (status == SQLITE_ROW) {
			for (auto it = query.begin(); it != query.end(); ++it) {
				auto row = *it;
				this->main_quests.push_back(
					new ServerQuest(
						database,
						row["id"],
						this,
						row["caption"],
						nullptr,
						std::vector<Quest*>()));
			}
		}

	}

}
