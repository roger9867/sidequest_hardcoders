#include "server_user.h"
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

		auto query = database->create_query("INSERT INTO user(email, display_name, password) VALUES (?, ?, ?);");

		query->bind(1, email);
		query->bind(2, display_name);
		query->bind(3, password);

		if (query->execute() != SQLITE_DONE)
			throw UnableToCreateObjectException(email);

		// Reset statement??
	}


	void ServerUser::read_on_database() {

		auto query = database->create_query("SELECT * FROM user WHERE email = ?;");

		query->bind(1, email);
		if (query->execute() != SQLITE_ROW)
			throw UnableToReadObjectException(email);
		display_name = query->read_text_value("display_name");
		password     = query->read_text_value("password");
		/*
		database->bind(prepared_statement, 1, email);
		if ( database->execute(prepared_statement) != SQLITE_ROW )
			throw UnableToReadObjectException(email);
		display_name = database->read_text_value(prepared_statement, "display_name");
		password     = database->read_text_value(prepared_statement, "password");
		database->reset_statement(prepared_statement);
		*/

		// Reset??
	}

	/*
	void ServerUser::update_on_database() {
		auto prepared_statement = database->prepare("UPDATE user set display_name=?, password=? WHERE email=?;");
		database->bind(prepared_statement, 1, display_name);
		database->bind(prepared_statement, 2, password);
		database->bind(prepared_statement, 3, email);
		if ( database->execute(prepared_statement) != SQLITE_DONE )
			throw UnableToUpdateObjectException(email);
		database->reset_statement(prepared_statement);
	}

	void ServerUser::delete_on_database() {
		auto prepared_statement = database->prepare("DELETE FROM user WHERE email=?;");
		database->bind(prepared_statement, 1, email);
		if (database->execute(prepared_statement) != SQLITE_DONE)
			throw UnableToDeleteObjectException(email);
		database->reset_statement(prepared_statement);
	}

	std::string ServerUser::class_id() {
		return "user";
	}
*/
}
