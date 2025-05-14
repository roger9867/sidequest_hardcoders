#pragma once

#include <string>
#include <vector>

#include <model/user.h>
#include <storage/persistable.h>

namespace Sidequest
{
	namespace Server
	{

		class ServerUser : public Sidequest::User, public Persistable {
		public:
			typedef unsigned long Id;

			ServerUser(Database* database);
			ServerUser(Database* database, std::string email);
			ServerUser(Database* database, std::string email, std::string display_name, std::string password);
			~ServerUser();

			virtual void create_on_database() override;
			virtual void read_on_database() override;
			virtual void update_on_database() override;
			virtual void delete_on_database() override;

			virtual std::string class_id() override;
		};

	}
}
