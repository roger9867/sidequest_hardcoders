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

TEST_F(ServerUserTests, LOAD_MAINQUESTS) {

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

TEST_F(ServerUserTests, LOAD_SUBQUESTS) {
	auto user = new ServerUser(database, "user@email.com", "Test user", "the_password");

	user->add_main_quests(
		new ServerQuest(database, "id1", user, "caption1", nullptr, {}),
		new ServerQuest(database, "id2", user, "caption2", nullptr, {})
	);

	auto subquests1 = std::vector<Sidequest::Quest*> {
		new ServerQuest(database, "id1.1", user, "caption1.1", user->get_main_quests()[0], {}),
		new ServerQuest(database, "id1.2", user, "caption1.2", user->get_main_quests()[0], {})
	};
	auto subquests2 = std::vector<Sidequest::Quest*> {
		new ServerQuest(database, "id2.1", user, "caption2.1", user->get_main_quests()[1], {}),
		new ServerQuest(database, "id2.2", user, "caption2.2", user->get_main_quests()[1], {})
	};

	// set subquests on main quests
	user->get_main_quests()[0]->set_subquests(subquests1);
	user->get_main_quests()[1]->set_subquests(subquests2);

	// user->create_on_database(); // owner is persisted without doing it explicitly!
	for (auto main_quest : user->get_main_quests()) {
		auto server_quest = ServerQuest(database,
			main_quest->get_id(),
			main_quest->get_owner(),
			main_quest->get_caption(),
			main_quest->get_parent(),
			main_quest->get_subquests());
		server_quest.create_on_database();
		for (auto subquest : main_quest->get_subquests()) {
			auto server_subquest = ServerQuest(database,
			subquest->get_id(),
			subquest->get_owner(),
			subquest->get_caption(),
			subquest->get_parent(),
			subquest->get_subquests());
			server_subquest.create_on_database();
		}
	}

	delete user;
	user = nullptr;
	auto user2 = new ServerUser(database, "user@email.com");
	user2->read_on_database();
	user2->load_main_quests();

	EXPECT_EQ(user2->get_main_quests().size(), 2);

	// load subquests for each main quest
    for (auto main_quest : user2->get_main_quests()) {
        auto server_quest = ServerQuest(
            database,
            main_quest->get_id(),
            main_quest->get_owner(),
            main_quest->get_caption(),
            main_quest->get_parent(),
            main_quest->get_subquests());
        server_quest.load_sub_quests();
        main_quest->set_subquests(server_quest.get_subquests());
    }

    // ensure the main quest has subquests loaded
    auto main_quest = user2->get_main_quests()[0];

    EXPECT_FALSE(main_quest->get_subquests().empty())
        << "Expected main quest to have subquests, but found none.";

    // check the first subquest
    auto main_subquest1_1 = main_quest->get_subquests().at(0);
    EXPECT_EQ(main_subquest1_1->get_id(), "id1.1");
    EXPECT_EQ(main_subquest1_1->get_owner()->get_email(), "user@email.com");
    EXPECT_EQ(main_subquest1_1->get_caption(), "caption1.1");
    EXPECT_EQ(main_subquest1_1->get_parent()->get_id(), "id1");
    EXPECT_EQ(main_subquest1_1->get_subquests().size(), 0);

    // check all fields for the first subquest
    EXPECT_EQ(main_subquest1_1->get_id(), "id1.1");
    EXPECT_EQ(main_subquest1_1->get_owner()->get_email(), "user@email.com");
    EXPECT_EQ(main_subquest1_1->get_caption(), "caption1.1");
    EXPECT_EQ(main_subquest1_1->get_parent()->get_id(), "id1");
    EXPECT_EQ(main_subquest1_1->get_subquests().size(), 0);

    // check the second subquest
    EXPECT_EQ(main_quest->get_subquests().size(), 2);

    auto main_subquest1_2 = main_quest->get_subquests().at(1);
    EXPECT_EQ(main_subquest1_2->get_id(), "id1.2");
    EXPECT_EQ(main_subquest1_2->get_owner()->get_email(), "user@email.com");
    EXPECT_EQ(main_subquest1_2->get_caption(), "caption1.2");
    EXPECT_EQ(main_subquest1_2->get_parent()->get_id(), "id1");
    EXPECT_EQ(main_subquest1_2->get_subquests().size(), 0);

    // check all fields for the second subquest
    EXPECT_EQ(main_subquest1_2->get_id(), "id1.2");
    EXPECT_EQ(main_subquest1_2->get_owner()->get_email(), "user@email.com");
    EXPECT_EQ(main_subquest1_2->get_caption(), "caption1.2");
    EXPECT_EQ(main_subquest1_2->get_parent()->get_id(), "id1");
    EXPECT_EQ(main_subquest1_2->get_subquests().size(), 0);

    // check subquests for the second main quest
    auto main_subquest2_1 = user2->get_main_quests()[1]->get_subquests().at(0);
    EXPECT_EQ(main_subquest2_1->get_id(), "id2.1");
    EXPECT_EQ(main_subquest2_1->get_owner()->get_email(), "user@email.com");
    EXPECT_EQ(main_subquest2_1->get_caption(), "caption2.1");
    EXPECT_EQ(main_subquest2_1->get_parent()->get_id(), "id2");
    EXPECT_EQ(main_subquest2_1->get_subquests().size(), 0);

    auto main_subquest2_2 = user2->get_main_quests()[1]->get_subquests().at(1);
    EXPECT_EQ(main_subquest2_2->get_id(), "id2.2");
    EXPECT_EQ(main_subquest2_2->get_owner()->get_email(), "user@email.com");
    EXPECT_EQ(main_subquest2_2->get_caption(), "caption2.2");
    EXPECT_EQ(main_subquest2_2->get_parent()->get_id(), "id2");
    EXPECT_EQ(main_subquest2_2->get_subquests().size(), 0);
}



