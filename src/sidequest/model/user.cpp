#include "user.h"

namespace Sidequest  {

	User::User() {}

	User::User( std::string email)
		: email(email) {}

	User::User(std::string email, std::string display_name, std::string password)
		: email(email), display_name(display_name), password(password) {}

	User::~User() {}


	std::string User::get_email() const {
		return email;
	}
	std::string User::get_display_name() const {
		return display_name;
	}
	std::string User::get_password() const {
		return password;
	}
	void User::set_password(std::string password) {
		this->password = password;
	}
	void User::set_display_name(std::string display_name) {
		this->display_name = display_name;
	}
	void User::set_email(std::string email) {
		this->email = email;
	}

}