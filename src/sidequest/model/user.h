#pragma once

#include <string>
#include <vector>

namespace Sidequest
{
	class Quest;

	class User {
	public:
		User();
		User(std::string email);
		User(std::string email, std::string display_name, std::string password);
		~User();

		std::string display_name;
		std::string email;
		std::string password;

		std::vector<Quest*> main_quests;
	};
}