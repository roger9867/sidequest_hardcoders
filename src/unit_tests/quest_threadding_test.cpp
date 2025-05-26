//
// Created by roger on 5/26/25.
//
#include <gtest/gtest.h>
#include <filesystem>

#include "model/server_quest.h"
#include "storage/database.h"
#include "storage/database_exceptions.h"
#include "model/server_user.h"


class QueryThreadTests : public ::testing::Test
{
protected:
	Sidequest::Server::Database* database;

	QueryThreadTests()
	{
	}

	virtual ~QueryThreadTests() {
	}

	virtual void SetUp() {
		database = new Sidequest::Server::Database(":memory:");
	}
	virtual void TearDown() {
		delete database;
	}
};

using namespace Sidequest::Server;

TEST_F(QueryThreadTests, TEST_THREAD) {

	auto user = new ServerUser(database,"user@email.com", "Test user", "the_password" );
	user->add_main_quests(
		new ServerQuest(database, "id1", user, "caption1", nullptr, std::vector<Sidequest::Quest*>()),
		new ServerQuest(database, "id2", user, "caption2", nullptr, std::vector<Sidequest::Quest*>()),
		new ServerQuest(database, "id3", user, "caption3", nullptr, std::vector<Sidequest::Quest*>()),
		new ServerQuest(database, "id4", user, "caption4", nullptr, std::vector<Sidequest::Quest*>())
	);

	//user->create_on_database(); // owner is persisted without doing it explicitly!
	for (auto main_quest : user->get_main_quests()) {
		auto server_quest = ServerQuest(database,
			main_quest->get_id(),
			main_quest->get_owner(),
			main_quest->get_caption(),
			main_quest->get_parent(),
			main_quest->get_subquests());
		server_quest.create_on_database();
	}

	delete user;
	user = nullptr;
	auto user2 = new ServerUser(database, "user@email.com");
	user2->read_on_database();
	user2->load_main_quests();

	EXPECT_EQ(user2->get_main_quests().size(), 4);

	EXPECT_EQ(user2->get_main_quests()[0]->get_id(), "id1");
	EXPECT_EQ(user2->get_main_quests()[0]->get_owner()->get_email(), "user@email.com");
	EXPECT_EQ(user2->get_main_quests()[0]->get_caption(), "caption1");
	EXPECT_EQ(user2->get_main_quests()[0]->get_parent(), nullptr);
	EXPECT_EQ(user2->get_main_quests()[0]->get_subquests(), std::vector<Sidequest::Quest*>());

	EXPECT_EQ(user2->get_main_quests()[1]->get_id(), "id2");
	EXPECT_EQ(user2->get_main_quests()[1]->get_owner()->get_email(), "user@email.com");
	EXPECT_EQ(user2->get_main_quests()[1]->get_caption(), "caption2");
	EXPECT_EQ(user2->get_main_quests()[1]->get_parent(), nullptr);
	EXPECT_EQ(user2->get_main_quests()[1]->get_subquests(), std::vector<Sidequest::Quest*>());

	EXPECT_EQ(user2->get_main_quests()[2]->get_id(), "id3");
	EXPECT_EQ(user2->get_main_quests()[2]->get_owner()->get_email(), "user@email.com");
	EXPECT_EQ(user2->get_main_quests()[2]->get_caption(), "caption3");
	EXPECT_EQ(user2->get_main_quests()[2]->get_parent(), nullptr);
	EXPECT_EQ(user2->get_main_quests()[2]->get_subquests(), std::vector<Sidequest::Quest*>());

	EXPECT_EQ(user2->get_main_quests()[3]->get_id(), "id4");
	EXPECT_EQ(user2->get_main_quests()[3]->get_owner()->get_email(), "user@email.com");
	EXPECT_EQ(user2->get_main_quests()[3]->get_caption(), "caption4");
	EXPECT_EQ(user2->get_main_quests()[3]->get_parent(), nullptr);
	EXPECT_EQ(user2->get_main_quests()[3]->get_subquests(), std::vector<Sidequest::Quest*>());
}