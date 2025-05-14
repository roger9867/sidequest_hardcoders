#include "user.h"

namespace Sidequest 
{

	User::User()
	{
	}

	User::User( std::string email)
		: email(email)
	{
	}

	User::User(std::string email, std::string display_name, std::string password)
		: email(email), display_name(display_name), password(password)
	{
	}

	User::~User()
	{
	}
}