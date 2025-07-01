#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

namespace Sidequest {

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
		virtual ~User();

		std::string get_id() const;
		std::string get_email() const;
		std::string get_display_name() const;
		std::string get_password() const;
		std::vector<Quest*> get_main_quests() const;

		void set_password(std::string password);
		void set_display_name(std::string display_name);
		void set_email(std::string email);


		void add_main_quests(Quest* quest);

		// Recursive case: more than one Quest*
		template<typename... Quests>
		void add_main_quests(Quest* first, Quests... rest) {
			main_quests.push_back(first);
			add_main_quests(rest...);  // recursive call
		}
	};

}  // namespace Sidequest

#endif  //USER_H
