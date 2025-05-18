#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

namespace Sidequest
{
	class Quest;

	class User {
	protected:
		std::string display_name;
		std::string email;
		std::string password;
	public:

		std::vector<Quest*> main_quests = std::vector<Quest*>();

		User();
		User(std::string email);
		User(std::string email, std::string display_name, std::string password);
		~User();

		std::string get_email() const;
		std::string get_display_name() const;
		std::string get_password() const;

		void set_password(std::string password);
		void set_display_name(std::string display_name);
		void set_email(std::string email);
	};
}

#endif  //USER_H
