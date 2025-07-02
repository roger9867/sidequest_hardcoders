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
	auto editor = new ServerUser(database, "editor@email.com", "Test editor", "the_password" );
	user->add_main_quests(
		new ServerQuest(database, "id1", Sidequest::Quest::initial, "title1", "caption1", user, user, nullptr, std::vector<Sidequest::Quest*>()),
		new ServerQuest(database, "id2", Sidequest::Quest::initial, "title2", "caption2", user, user, nullptr, std::vector<Sidequest::Quest*>()),
		new ServerQuest(database, "id3", Sidequest::Quest::done, "title3", "caption3", user, editor, nullptr, std::vector<Sidequest::Quest*>()),
		new ServerQuest(database, "id4", Sidequest::Quest::inactive, "title4", "caption4", user, editor, nullptr, std::vector<Sidequest::Quest*>())

	);

	//user->create_on_database(); // owner and editor are persisted without doing it explicitly!
	for (auto main_quest : user->get_main_quests()) {
		auto server_quest = ServerQuest(
			database,
			main_quest->get_id(),
			main_quest->get_status(),
			main_quest->get_title(),
			main_quest->get_caption(),
			main_quest->get_owner(),
			main_quest->get_editor(),
			main_quest->get_parent(),
			main_quest->get_subquests());
		server_quest.create_on_database();
	}

	delete(user);
	user = nullptr;
	auto user2 = new ServerUser(database, "user@email.com");
	user2->read_on_database();
	user2->load_main_quests();

	EXPECT_EQ(user2->get_main_quests().size(), 4);

	EXPECT_EQ(user2->get_main_quests()[0]->get_id(), "id1");
	EXPECT_EQ(user2->get_main_quests()[0]->get_status(), Sidequest::Quest::initial);
	EXPECT_EQ(user2->get_main_quests()[0]->get_title(), "title1");
	EXPECT_EQ(user2->get_main_quests()[0]->get_caption(), "caption1");
	EXPECT_EQ(user2->get_main_quests()[0]->get_owner()->get_email(), "user@email.com");
	EXPECT_EQ(user2->get_main_quests()[0]->get_editor()->get_email(), "user@email.com");
	EXPECT_EQ(user2->get_main_quests()[0]->get_parent(), nullptr);
	EXPECT_EQ(user2->get_main_quests()[0]->get_subquests(), std::vector<Sidequest::Quest*>());

	EXPECT_EQ(user2->get_main_quests()[1]->get_id(), "id2");
	EXPECT_EQ(user2->get_main_quests()[1]->get_status(), Sidequest::Quest::initial);
	EXPECT_EQ(user2->get_main_quests()[1]->get_title(), "title2");
	EXPECT_EQ(user2->get_main_quests()[1]->get_caption(), "caption2");
	EXPECT_EQ(user2->get_main_quests()[1]->get_owner()->get_email(), "user@email.com");
	EXPECT_EQ(user2->get_main_quests()[1]->get_editor()->get_email(), "user@email.com");
	EXPECT_EQ(user2->get_main_quests()[1]->get_parent(), nullptr);
	EXPECT_EQ(user2->get_main_quests()[1]->get_subquests(), std::vector<Sidequest::Quest*>());

	EXPECT_EQ(user2->get_main_quests()[2]->get_id(), "id3");
	EXPECT_EQ(user2->get_main_quests()[2]->get_status(), Sidequest::Quest::done);
	EXPECT_EQ(user2->get_main_quests()[2]->get_title(), "title3");
	EXPECT_EQ(user2->get_main_quests()[2]->get_caption(), "caption3");
	EXPECT_EQ(user2->get_main_quests()[2]->get_owner()->get_email(), "user@email.com");
	EXPECT_EQ(user2->get_main_quests()[2]->get_editor()->get_email(), "editor@email.com");
	EXPECT_EQ(user2->get_main_quests()[2]->get_parent(), nullptr);
	EXPECT_EQ(user2->get_main_quests()[2]->get_subquests(), std::vector<Sidequest::Quest*>());

	EXPECT_EQ(user2->get_main_quests()[3]->get_id(), "id4");
	EXPECT_EQ(user2->get_main_quests()[3]->get_status(), Sidequest::Quest::inactive);
	EXPECT_EQ(user2->get_main_quests()[3]->get_title(), "title4");
	EXPECT_EQ(user2->get_main_quests()[3]->get_caption(), "caption4");
	EXPECT_EQ(user2->get_main_quests()[3]->get_owner()->get_email(), "user@email.com");
	EXPECT_EQ(user2->get_main_quests()[3]->get_editor()->get_email(), "editor@email.com");
	EXPECT_EQ(user2->get_main_quests()[3]->get_parent(), nullptr);
	EXPECT_EQ(user2->get_main_quests()[3]->get_subquests(), std::vector<Sidequest::Quest*>());
}




TEST_F(ServerUserTests, LOAD_SUBQUESTS) {

    auto user = new ServerUser(database,"user@email.com", "Test user", "the_password" );
    auto editor = new ServerUser(database, "editor@email.com", "Test editor", "the_password" );

    user->add_main_quests(
        new ServerQuest(database, "id1", Sidequest::Quest::done, "title1", "caption1", user, user, nullptr, {}),
        new ServerQuest(database, "id2", Sidequest::Quest::done, "title2", "caption2", user, editor, nullptr, {})
    );

    auto subquests1 = std::vector<Sidequest::Quest*> {
        new ServerQuest(database, "id1.1", Sidequest::Quest::done, "title1.1", "caption1.1", user, editor, user->get_main_quests()[0], {}),
        new ServerQuest(database, "id1.2", Sidequest::Quest::done, "title1.2", "caption1.2", user, editor, user->get_main_quests()[0], {})
    };
    auto subquests2 = std::vector<Sidequest::Quest*> {
        new ServerQuest(database, "id2.1", Sidequest::Quest::done, "title2.1", "caption2.1", user, editor, user->get_main_quests()[1], {}),
        new ServerQuest(database, "id2.2", Sidequest::Quest::done, "title2.2", "caption2.2", user, editor, user->get_main_quests()[1], {})
    };

    // set subquests on main quests
    user->get_main_quests()[0]->set_subquests(subquests1);
    user->get_main_quests()[1]->set_subquests(subquests2);

    // All subquests are definitely loaded
    ASSERT_EQ(user->get_main_quests()[0]->get_subquests().size(), 2);
    ASSERT_EQ(user->get_main_quests()[1]->get_subquests().size(), 2);

    // Persist main quests and their subquests to database
    for (auto main_quest : user->get_main_quests()) {
        // Convert main quest to ServerQuest and persist
        auto server_quest = ServerQuest(
            database,
            main_quest->get_id(),
            main_quest->get_status(),
            main_quest->get_title(),
            main_quest->get_caption(),
            main_quest->get_owner(),
            main_quest->get_editor(),
            main_quest->get_parent(),
            main_quest->get_subquests()
        );
        server_quest.create_on_database();

        // Persist each subquest individually with their own data
        for (auto subquest : main_quest->get_subquests()) {
            auto server_subquest = ServerQuest(
                database,
                subquest->get_id(),
                subquest->get_status(),
                subquest->get_title(),
                subquest->get_caption(),
                subquest->get_owner(),
                subquest->get_editor(),
                subquest->get_parent(),
                subquest->get_subquests()
            );
            server_subquest.create_on_database();
        }
    }

    delete user;
    user = nullptr;

    auto user2 = ServerUser(database, "user@email.com");
    user2.read_on_database();
    user2.load_main_quests();

    EXPECT_EQ(user2.get_main_quests().size(), 2);

    // load subquests for each main quest
    for (auto main_quest : user2.get_main_quests()) {
        auto server_quest = ServerQuest(
            database,
            main_quest->get_id(),
            main_quest->get_status(),
            main_quest->get_title(),
            main_quest->get_caption(),
            main_quest->get_owner(),
            main_quest->get_editor(),
            main_quest->get_parent(),
            main_quest->get_subquests()
        );

        server_quest.load_sub_quests();
        main_quest->set_subquests(server_quest.get_subquests());
    }

    // ensure the main quest has subquests loaded
    auto main_quest = user2.get_main_quests()[0];

    EXPECT_FALSE(main_quest->get_subquests().empty())
        << "Expected main quest to have subquests, but found none.";

    // check the first subquest
    auto main_subquest1_1 = main_quest->get_subquests().at(0);
    EXPECT_EQ(main_subquest1_1->get_id(), "id1.1");
    EXPECT_EQ(main_subquest1_1->get_status(), Sidequest::Quest::done);

    EXPECT_EQ(main_subquest1_1->get_caption(), "caption1.1");
    EXPECT_EQ(main_subquest1_1->get_owner()->get_email(), "user@email.com");
    EXPECT_EQ(main_subquest1_1->get_editor()->get_email(), "editor@email.com");

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
    auto main_subquest2_1 = user2.get_main_quests()[1]->get_subquests().at(0);
    EXPECT_EQ(main_subquest2_1->get_id(), "id2.1");
    EXPECT_EQ(main_subquest2_1->get_owner()->get_email(), "user@email.com");
    EXPECT_EQ(main_subquest2_1->get_caption(), "caption2.1");
    EXPECT_EQ(main_subquest2_1->get_parent()->get_id(), "id2");
    EXPECT_EQ(main_subquest2_1->get_subquests().size(), 0);

    auto main_subquest2_2 = user2.get_main_quests()[1]->get_subquests().at(1);
    EXPECT_EQ(main_subquest2_2->get_id(), "id2.2");
    EXPECT_EQ(main_subquest2_2->get_owner()->get_email(), "user@email.com");
    EXPECT_EQ(main_subquest2_2->get_caption(), "caption2.2");
    EXPECT_EQ(main_subquest2_2->get_parent()->get_id(), "id2");
    EXPECT_EQ(main_subquest2_2->get_subquests().size(), 0);
}

TEST_F(ServerUserTests, LOAD_SUBQUESTS_EXTENDED) {
	auto user = new ServerUser(database, "user@email.com", "Test user 1", "the_password_1");
	auto editor = new ServerUser(database, "editor@email.com", "Test editor 1", "the_password_1" );

	user->add_main_quests(
	new ServerQuest(database, "id1", Sidequest::Quest::initial, "title1", "caption1", user, editor, nullptr, {}),
	new ServerQuest(database, "id2", Sidequest::Quest::initial, "title2", "caption2", user, editor, nullptr, {}),
	new ServerQuest(database, "id3", Sidequest::Quest::initial, "title3", "caption3", user, editor, nullptr, {})
	);

	auto subquests1 = std::vector<Sidequest::Quest*> {
    new ServerQuest(database, "id1.1", Sidequest::Quest::initial, "title1.1", "caption1.1", user, editor, user->get_main_quests()[0], {}),
    new ServerQuest(database, "id1.2", Sidequest::Quest::initial, "title1.2", "caption1.2", user, editor, user->get_main_quests()[0], {}),
    new ServerQuest(database, "id1.3", Sidequest::Quest::initial, "title1.3", "caption1.3", user, editor, user->get_main_quests()[0], {})
};

auto sub_subquests11 = std::vector<Sidequest::Quest*> {
    new ServerQuest(database, "id1.1.1", Sidequest::Quest::initial, "title1.1.1", "caption1.1.1", user, editor, subquests1[0], {}),
    new ServerQuest(database, "id1.1.2", Sidequest::Quest::initial, "title1.1.2", "caption1.1.2", user, editor, subquests1[0], {}),
    new ServerQuest(database, "id1.1.3", Sidequest::Quest::initial, "title1.1.3", "caption1.1.3", user, editor, subquests1[0], {})
};

auto sub_subquests12 = std::vector<Sidequest::Quest*> {
    new ServerQuest(database, "id1.2.1", Sidequest::Quest::initial, "title1.2.1", "caption1.2.1", user, editor, subquests1[1], {}),
    new ServerQuest(database, "id1.2.2", Sidequest::Quest::initial, "title1.2.2", "caption1.2.2", user, editor, subquests1[1], {}),
    new ServerQuest(database, "id1.2.3", Sidequest::Quest::initial, "title1.2.3", "caption1.2.3", user, editor, subquests1[1], {})
};

auto sub_subquests13 = std::vector<Sidequest::Quest*> {
    new ServerQuest(database, "id1.3.1", Sidequest::Quest::initial, "title1.3.1", "caption1.3.1", user, editor, subquests1[2], {}),
    new ServerQuest(database, "id1.3.2", Sidequest::Quest::initial, "title1.3.2", "caption1.3.2", user, editor, subquests1[2], {}),
    new ServerQuest(database, "id1.3.3", Sidequest::Quest::initial, "title1.3.3", "caption1.3.3", user, editor, subquests1[2], {})
};

auto subquests2 = std::vector<Sidequest::Quest*> {
    new ServerQuest(database, "id2.1", Sidequest::Quest::initial, "title2.1", "caption2.1", user, editor, user->get_main_quests()[1], {}),
    new ServerQuest(database, "id2.2", Sidequest::Quest::initial, "title2.2", "caption2.2", user, editor, user->get_main_quests()[1], {}),
    new ServerQuest(database, "id2.3", Sidequest::Quest::initial, "title2.3", "caption2.3", user, editor, user->get_main_quests()[1], {})
};

auto sub_subquests21 = std::vector<Sidequest::Quest*> {
    new ServerQuest(database, "id2.1.1", Sidequest::Quest::initial, "title2.1.1", "caption2.1.1", user, editor, subquests2[0], {}),
    new ServerQuest(database, "id2.1.2", Sidequest::Quest::initial, "title2.1.2", "caption2.1.2", user, editor, subquests2[0], {}),
    new ServerQuest(database, "id2.1.3", Sidequest::Quest::initial, "title2.1.3", "caption2.1.3", user, editor, subquests2[0], {})
};

auto sub_subquests22 = std::vector<Sidequest::Quest*> {
    new ServerQuest(database, "id2.2.1", Sidequest::Quest::initial, "title2.2.1", "caption2.2.1", user, editor, subquests2[1], {}),
    new ServerQuest(database, "id2.2.2", Sidequest::Quest::initial, "title2.2.2", "caption2.2.2", user, editor, subquests2[1], {}),
    new ServerQuest(database, "id2.2.3", Sidequest::Quest::initial, "title2.2.3", "caption2.2.3", user, editor, subquests2[1], {})
};

auto sub_subquests23 = std::vector<Sidequest::Quest*> {
    new ServerQuest(database, "id2.3.1", Sidequest::Quest::initial, "title2.3.1", "caption2.3.1", user, editor, subquests2[2], {}),
    new ServerQuest(database, "id2.3.2", Sidequest::Quest::initial, "title2.3.2", "caption2.3.2", user, editor, subquests2[2], {}),
    new ServerQuest(database, "id2.3.3", Sidequest::Quest::initial, "title2.3.3", "caption2.3.3", user, editor, subquests2[2], {})
};

auto subquests3 = std::vector<Sidequest::Quest*> {
    new ServerQuest(database, "id3.1", Sidequest::Quest::initial, "title3.1", "caption3.1", user, editor, user->get_main_quests()[2], {}),
    new ServerQuest(database, "id3.2", Sidequest::Quest::initial, "title3.2", "caption3.2", user, editor, user->get_main_quests()[2], {}),
    new ServerQuest(database, "id3.3", Sidequest::Quest::initial, "title3.3", "caption3.3", user, editor, user->get_main_quests()[2], {})
};

auto sub_subquests31 = std::vector<Sidequest::Quest*> {
    new ServerQuest(database, "id3.1.1", Sidequest::Quest::initial, "title3.1.1", "caption3.1.1", user, editor, subquests3[0], {}),
    new ServerQuest(database, "id3.1.2", Sidequest::Quest::initial, "title3.1.2", "caption3.1.2", user, editor, subquests3[0], {}),
    new ServerQuest(database, "id3.1.3", Sidequest::Quest::initial, "title3.1.3", "caption3.1.3", user, editor, subquests3[0], {})
};

auto sub_subquests32 = std::vector<Sidequest::Quest*> {
    new ServerQuest(database, "id3.2.1", Sidequest::Quest::initial, "title3.2.1", "caption3.2.1", user, editor, subquests3[1], {}),
    new ServerQuest(database, "id3.2.2", Sidequest::Quest::initial, "title3.2.2", "caption3.2.2", user, editor, subquests3[1], {}),
    new ServerQuest(database, "id3.2.3", Sidequest::Quest::initial, "title3.2.3", "caption3.2.3", user, editor, subquests3[1], {})
};

auto sub_subquests33 = std::vector<Sidequest::Quest*> {
    new ServerQuest(database, "id3.3.1", Sidequest::Quest::initial, "title3.3.1", "caption3.3.1", user, editor, subquests3[2], {}),
    new ServerQuest(database, "id3.3.2", Sidequest::Quest::initial, "title3.3.2", "caption3.3.2", user, editor, subquests3[2], {}),
    new ServerQuest(database, "id3.3.3", Sidequest::Quest::initial, "title3.3.3", "caption3.3.3", user, editor, subquests3[2], {})
};


	// set subquests on main quests
	user->get_main_quests()[0]->set_subquests(subquests1);
		user->get_main_quests()[0]->get_subquests()[0]->set_subquests(sub_subquests11);
		user->get_main_quests()[0]->get_subquests()[1]->set_subquests(sub_subquests12);
		user->get_main_quests()[0]->get_subquests()[2]->set_subquests(sub_subquests13);
	user->get_main_quests()[1]->set_subquests(subquests2);
		user->get_main_quests()[1]->get_subquests()[0]->set_subquests(sub_subquests21);
		user->get_main_quests()[1]->get_subquests()[1]->set_subquests(sub_subquests22);
		user->get_main_quests()[1]->get_subquests()[2]->set_subquests(sub_subquests23);
	user->get_main_quests()[2]->set_subquests(subquests3);
		user->get_main_quests()[2]->get_subquests()[0]->set_subquests(sub_subquests31);
		user->get_main_quests()[2]->get_subquests()[1]->set_subquests(sub_subquests32);
		user->get_main_quests()[2]->get_subquests()[2]->set_subquests(sub_subquests33);

	// All subquests are definitely loaded
	ASSERT_EQ(user->get_main_quests()[0]->get_subquests().size(), 3);
		ASSERT_EQ(user->get_main_quests()[0]->get_subquests()[0]->get_subquests().size(), 3);
		ASSERT_EQ(user->get_main_quests()[0]->get_subquests()[1]->get_subquests().size(), 3);
		ASSERT_EQ(user->get_main_quests()[0]->get_subquests()[2]->get_subquests().size(), 3);
	ASSERT_EQ(user->get_main_quests()[1]->get_subquests().size(), 3);
		ASSERT_EQ(user->get_main_quests()[1]->get_subquests()[0]->get_subquests().size(), 3);
		ASSERT_EQ(user->get_main_quests()[1]->get_subquests()[1]->get_subquests().size(), 3);
		ASSERT_EQ(user->get_main_quests()[1]->get_subquests()[2]->get_subquests().size(), 3);
	ASSERT_EQ(user->get_main_quests()[2]->get_subquests().size(), 3);
		ASSERT_EQ(user->get_main_quests()[2]->get_subquests()[0]->get_subquests().size(), 3);
		ASSERT_EQ(user->get_main_quests()[2]->get_subquests()[1]->get_subquests().size(), 3);
		ASSERT_EQ(user->get_main_quests()[2]->get_subquests()[2]->get_subquests().size(), 3);








	// user->create_on_database(); // owner is persisted without doing it explicitly!
	for (auto main_quest : user->get_main_quests()) {

		auto server_quest = ServerQuest(
			database,
			main_quest->get_id(),
			main_quest->get_status(),
			main_quest->get_title(),
			main_quest->get_caption(),
			main_quest->get_owner(),
			main_quest->get_editor(),
			main_quest->get_parent(),
			main_quest->get_subquests());
		server_quest.create_on_database();

		for (auto subquest : main_quest->get_subquests()) {
			auto server_subquest = ServerQuest(
				database,
				subquest->get_id(),
				subquest->get_status(),
				subquest->get_title(),
				subquest->get_caption(),
				subquest->get_owner(),
				subquest->get_editor(),
				subquest->get_parent(),
				subquest->get_subquests());
			server_subquest.create_on_database();

			for (auto subsubquest : subquest->get_subquests()) {
				auto server_subsubquest = ServerQuest(
					database,
					subsubquest->get_id(),
					subsubquest->get_status(),
					subsubquest->get_title(),
					subsubquest->get_caption(),
					subsubquest->get_owner(),
					subsubquest->get_editor(),
					subsubquest->get_parent(),
					subsubquest->get_subquests());
				server_subsubquest.create_on_database();
			}
		}
	}

			delete user;
			user = nullptr;






			auto user2 = ServerUser(database, "user@email.com");
			user2.read_on_database();
			user2.load_main_quests();

			ASSERT_EQ(user2.get_main_quests().size(), 3);
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests().size(), 0);
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests().size(), 0);
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests().size(), 0);


			// load subquests for each main quest
			for (auto main_quest : user2.get_main_quests()) {
				auto server_quest = ServerQuest(
					database,
					main_quest->get_id(),
					main_quest->get_status(),
					main_quest->get_title(),
					main_quest->get_caption(),
					main_quest->get_owner(),
					main_quest->get_editor(),
					main_quest->get_parent(),
					main_quest->get_subquests());
				server_quest.load_sub_quests();
				main_quest->set_subquests(server_quest.get_subquests());
			}

			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests().size(), 3);
				ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests().size(), 0);
				ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests().size(), 0);
				ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests().size(), 0);

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests().size(), 3);
				ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests().size(), 0);
				ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests().size(), 0);
				ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests().size(), 0);

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests().size(), 3);
				ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests().size(), 0);
				ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests().size(), 0);
				ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests().size(), 0);


			for (auto mainquest : user2.get_main_quests()) {
				for (auto subquest : mainquest->get_subquests()) {
					auto server_quest = ServerQuest(database,
						subquest->get_id(),
						subquest->get_status(),
						subquest->get_title(),
						subquest->get_caption(),
						subquest->get_owner(),
						subquest->get_editor(),
						subquest->get_parent(),
						subquest->get_subquests());
					server_quest.load_sub_quests();
					subquest->set_subquests(server_quest.get_subquests());
				}
			}

	ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests().size(), 3);
	ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests().size(), 3);
	ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests().size(), 3);

	ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests().size(), 3);
	ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests().size(), 3);
	ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests().size(), 3);

	ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests().size(), 3);
	ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests().size(), 3);
	ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests().size(), 3);


	// Check all (sub-)quests
	ASSERT_EQ(user2.get_main_quests()[0]->get_id(), "id1");
	ASSERT_EQ(user2.get_main_quests()[1]->get_id(), "id2");
	ASSERT_EQ(user2.get_main_quests()[2]->get_id(), "id3");

	ASSERT_EQ(user2.get_main_quests()[0]->get_status(), Sidequest::Quest::initial);
	ASSERT_EQ(user2.get_main_quests()[1]->get_status(), Sidequest::Quest::initial);
	ASSERT_EQ(user2.get_main_quests()[2]->get_status(), Sidequest::Quest::initial);

	ASSERT_EQ(user2.get_main_quests()[0]->get_title(), "title1");
	ASSERT_EQ(user2.get_main_quests()[1]->get_title(), "title2");
	ASSERT_EQ(user2.get_main_quests()[2]->get_title(), "title3");

	ASSERT_EQ(user2.get_main_quests()[0]->get_caption(), "caption1");
	ASSERT_EQ(user2.get_main_quests()[1]->get_caption(), "caption2");
	ASSERT_EQ(user2.get_main_quests()[2]->get_caption(), "caption3");

	ASSERT_EQ(user2.get_main_quests()[0]->get_owner()->get_email(), "user@email.com");
	ASSERT_EQ(user2.get_main_quests()[1]->get_owner()->get_email(), "user@email.com");
	ASSERT_EQ(user2.get_main_quests()[2]->get_owner()->get_email(), "user@email.com");

	ASSERT_EQ(user2.get_main_quests()[0]->get_editor()->get_email(), "editor@email.com");
	ASSERT_EQ(user2.get_main_quests()[1]->get_editor()->get_email(), "editor@email.com");
	ASSERT_EQ(user2.get_main_quests()[2]->get_editor()->get_email(), "editor@email.com");

		ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_id(), "id1.1");
		ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_id(), "id1.2");
		ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_id(), "id1.3");

		ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_caption(), "caption1.1");
		ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_caption(), "caption1.2");
		ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_caption(), "caption1.3");


			// check all subquests from 1st main quest
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[0]->get_id(), "id1.1.1");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[1]->get_id(), "id1.1.2");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[2]->get_id(), "id1.1.3");

			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[0]->get_status(), Sidequest::Quest::initial);
	        ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[1]->get_status(), Sidequest::Quest::initial);
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[2]->get_status(), Sidequest::Quest::initial);

			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[0]->get_title(), "title1.1.1");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[1]->get_title(), "title1.1.2");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[2]->get_title(), "title1.1.3");

			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[0]->get_caption(), "caption1.1.1");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[1]->get_caption(), "caption1.1.2");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[2]->get_caption(), "caption1.1.3");

			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[0]->get_owner()->get_email(), "user@email.com");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[1]->get_owner()->get_email(), "user@email.com");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[2]->get_owner()->get_email(), "user@email.com");

			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[0]->get_editor()->get_email(), "editor@email.com");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[1]->get_editor()->get_email(), "editor@email.com");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[2]->get_editor()->get_email(), "editor@email.com");


			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[0]->get_id(), "id1.2.1");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[1]->get_id(), "id1.2.2");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[2]->get_id(), "id1.2.3");

			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[0]->get_status(), Sidequest::Quest::initial);
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[1]->get_status(), Sidequest::Quest::initial);
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[2]->get_status(), Sidequest::Quest::initial);

			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[0]->get_title(), "title1.2.1");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[1]->get_title(), "title1.2.2");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[2]->get_title(), "title1.2.3");

			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[0]->get_caption(), "caption1.2.1");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[1]->get_caption(), "caption1.2.2");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[2]->get_caption(), "caption1.2.3");

			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[0]->get_owner()->get_email(), "user@email.com");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[1]->get_owner()->get_email(), "user@email.com");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[2]->get_owner()->get_email(), "user@email.com");

			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[0]->get_editor()->get_email(), "editor@email.com");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[1]->get_editor()->get_email(), "editor@email.com");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[2]->get_editor()->get_email(), "editor@email.com");

			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[0]->get_id(), "id1.3.1");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[1]->get_id(), "id1.3.2");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[2]->get_id(), "id1.3.3");

			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[0]->get_status(), Sidequest::Quest::initial);
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[1]->get_status(), Sidequest::Quest::initial);
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[2]->get_status(), Sidequest::Quest::initial);

			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[0]->get_title(), "title1.3.1");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[1]->get_title(), "title1.3.2");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[2]->get_title(), "title1.3.3");

			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[0]->get_caption(), "caption1.3.1");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[1]->get_caption(), "caption1.3.2");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[2]->get_caption(), "caption1.3.3");

			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[0]->get_owner()->get_email(), "user@email.com");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[1]->get_owner()->get_email(), "user@email.com");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[2]->get_owner()->get_email(), "user@email.com");

			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[0]->get_editor()->get_email(), "editor@email.com");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[1]->get_editor()->get_email(), "editor@email.com");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[2]->get_editor()->get_email(), "editor@email.com");


		ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_id(), "id2.1");
		ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_id(), "id2.2");
		ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_id(), "id2.3");

		ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_caption(), "caption2.1");
		ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_caption(), "caption2.2");
		ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_caption(), "caption2.3");


			// check all subquests from 2nd main quest
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[0]->get_id(), "id2.1.1");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[1]->get_id(), "id2.1.2");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[2]->get_id(), "id2.1.3");

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[0]->get_status(), Sidequest::Quest::initial);
	        ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[1]->get_status(), Sidequest::Quest::initial);
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[2]->get_status(), Sidequest::Quest::initial);

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[0]->get_title(), "title2.1.1");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[1]->get_title(), "title2.1.2");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[2]->get_title(), "title2.1.3");

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[0]->get_caption(), "caption2.1.1");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[1]->get_caption(), "caption2.1.2");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[2]->get_caption(), "caption2.1.3");

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[0]->get_owner()->get_email(), "user@email.com");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[1]->get_owner()->get_email(), "user@email.com");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[2]->get_owner()->get_email(), "user@email.com");

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[0]->get_editor()->get_email(), "editor@email.com");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[1]->get_editor()->get_email(), "editor@email.com");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[2]->get_editor()->get_email(), "editor@email.com");

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[0]->get_id(), "id2.2.1");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[1]->get_id(), "id2.2.2");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[2]->get_id(), "id2.2.3");

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[0]->get_status(), Sidequest::Quest::initial);
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[1]->get_status(), Sidequest::Quest::initial);
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[2]->get_status(), Sidequest::Quest::initial);

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[0]->get_title(), "title2.2.1");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[1]->get_title(), "title2.2.2");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[2]->get_title(), "title2.2.3");

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[0]->get_caption(), "caption2.2.1");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[1]->get_caption(), "caption2.2.2");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[2]->get_caption(), "caption2.2.3");

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[0]->get_owner()->get_email(), "user@email.com");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[1]->get_owner()->get_email(), "user@email.com");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[2]->get_owner()->get_email(), "user@email.com");

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[0]->get_editor()->get_email(), "editor@email.com");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[1]->get_editor()->get_email(), "editor@email.com");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[2]->get_editor()->get_email(), "editor@email.com");

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[0]->get_id(), "id2.3.1");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[1]->get_id(), "id2.3.2");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[2]->get_id(), "id2.3.3");

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[0]->get_status(), Sidequest::Quest::initial);
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[1]->get_status(), Sidequest::Quest::initial);
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[2]->get_status(), Sidequest::Quest::initial);

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[0]->get_title(), "title2.3.1");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[1]->get_title(), "title2.3.2");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[2]->get_title(), "title2.3.3");

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[0]->get_caption(), "caption2.3.1");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[1]->get_caption(), "caption2.3.2");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[2]->get_caption(), "caption2.3.3");

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[0]->get_owner()->get_email(), "user@email.com");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[1]->get_owner()->get_email(), "user@email.com");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[2]->get_owner()->get_email(), "user@email.com");

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[0]->get_editor()->get_email(), "editor@email.com");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[1]->get_editor()->get_email(), "editor@email.com");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[2]->get_editor()->get_email(), "editor@email.com");

		ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_id(), "id3.1");
		ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_id(), "id3.2");
		ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_id(), "id3.3");

		ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_caption(), "caption3.1");
		ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_caption(), "caption3.2");
		ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_caption(), "caption3.3");

			// check all subquests from 3rd main quest
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[0]->get_id(), "id3.1.1");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[1]->get_id(), "id3.1.2");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[2]->get_id(), "id3.1.3");

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[0]->get_status(), Sidequest::Quest::initial);
	        ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[1]->get_status(), Sidequest::Quest::initial);
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[2]->get_status(), Sidequest::Quest::initial);

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[0]->get_title(), "title3.1.1");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[1]->get_title(), "title3.1.2");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[2]->get_title(), "title3.1.3");

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[0]->get_caption(), "caption3.1.1");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[1]->get_caption(), "caption3.1.2");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[2]->get_caption(), "caption3.1.3");

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[0]->get_owner()->get_email(), "user@email.com");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[1]->get_owner()->get_email(), "user@email.com");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[2]->get_owner()->get_email(), "user@email.com");

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[0]->get_editor()->get_email(), "editor@email.com");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[1]->get_editor()->get_email(), "editor@email.com");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[2]->get_editor()->get_email(), "editor@email.com");

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[0]->get_id(), "id3.2.1");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[1]->get_id(), "id3.2.2");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[2]->get_id(), "id3.2.3");

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[0]->get_status(), Sidequest::Quest::initial);
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[1]->get_status(), Sidequest::Quest::initial);
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[2]->get_status(), Sidequest::Quest::initial);

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[0]->get_title(), "title3.2.1");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[1]->get_title(), "title3.2.2");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[2]->get_title(), "title3.2.3");

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[0]->get_caption(), "caption3.2.1");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[1]->get_caption(), "caption3.2.2");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[2]->get_caption(), "caption3.2.3");

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[0]->get_owner()->get_email(), "user@email.com");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[1]->get_owner()->get_email(), "user@email.com");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[2]->get_owner()->get_email(), "user@email.com");

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[0]->get_editor()->get_email(), "editor@email.com");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[1]->get_editor()->get_email(), "editor@email.com");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[2]->get_editor()->get_email(), "editor@email.com");

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[0]->get_id(), "id3.3.1");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[1]->get_id(), "id3.3.2");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[2]->get_id(), "id3.3.3");

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[0]->get_status(), Sidequest::Quest::initial);
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[1]->get_status(), Sidequest::Quest::initial);
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[2]->get_status(), Sidequest::Quest::initial);

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[0]->get_title(), "title3.3.1");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[1]->get_title(), "title3.3.2");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[2]->get_title(), "title3.3.3");

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[0]->get_caption(), "caption3.3.1");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[1]->get_caption(), "caption3.3.2");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[2]->get_caption(), "caption3.3.3");

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[0]->get_owner()->get_email(), "user@email.com");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[1]->get_owner()->get_email(), "user@email.com");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[2]->get_owner()->get_email(), "user@email.com");

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[0]->get_editor()->get_email(), "editor@email.com");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[1]->get_editor()->get_email(), "editor@email.com");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[2]->get_editor()->get_email(), "editor@email.com");

}
