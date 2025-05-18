#include <gtest/gtest.h>

#include <filesystem>

#include "model/server_quest.h"
#include "storage/database.h"
#include "storage/database_exceptions.h"
#include "model/server_user.h"

class ServerUserTests : public ::testing::Test
{
protected:
	Sidequest::Server::Database* database;

	ServerUserTests()
	{
	}

	virtual ~ServerUserTests() {
	}

	virtual void SetUp() {
		database = new Sidequest::Server::Database(":memory:");
	}
	virtual void TearDown() {
		delete database;
	}
};

using namespace Sidequest::Server;

TEST_F(ServerUserTests, OPEN_SERVER) {
	std::cout << "Working dir: " << std::filesystem::current_path() << std::endl;
}


