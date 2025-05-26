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

	// All subquests are definitely loaded
	ASSERT_EQ(user->get_main_quests()[0]->get_subquests().size(), 2);
	ASSERT_EQ(user->get_main_quests()[1]->get_subquests().size(), 2);
	//std::cout << user->get_main_quests()[0]->get_subquests().size() << std::endl;

	// user->create_on_database(); // owner is persisted without doing it explicitly!
	for (auto main_quest : user->get_main_quests()) {
		auto server_quest = ServerQuest(database,
			main_quest->get_id(),
			main_quest->get_owner(),
			main_quest->get_caption(),
			main_quest->get_parent(),
			main_quest->get_subquests());
			// create all main quests on database
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
	auto user2 = ServerUser(database, "user@email.com");
	user2.read_on_database();
	user2.load_main_quests();

	EXPECT_EQ(user2.get_main_quests().size(), 2);

	/*
	auto q = Query(database, "SELECT * FROM quest where parent_id=?");
	q.bind(1, "id1");

	for (auto it = q.begin(); it != q.end(); ++it)
		std::cout << "$$ " << (*it)["id"] << std::endl;
*/

	// load subquests for each main quest
    for (auto main_quest : user2.get_main_quests()) {
        auto server_quest = ServerQuest(
            database,
            main_quest->get_id(),
            main_quest->get_owner(),
            main_quest->get_caption(),
            main_quest->get_parent(),
            main_quest->get_subquests());

        server_quest.load_sub_quests();
    	// ---> Nach Load_Subquests nur 1 Subquest
    	std::cout << "§§ " << server_quest.get_subquests()[0]->get_id() << std::endl;
    	std::cout << "§§ " << server_quest.get_subquests()[1]->get_id() << std::endl;

        main_quest->set_subquests(server_quest.get_subquests());
    }

	std::cout << "?? " << user2.get_main_quests()[0]->get_subquests()[0]->get_id() << std::endl;
	std::cout << "?? " << user2.get_main_quests()[0]->get_subquests()[1]->get_id() << std::endl;

	/*
	auto q = Query(database, "SELECT * FROM quest where parent_id=?");
	q.bind(1, "id1");

	for (auto it = q.begin(); it != q.end(); ++it)
		std::cout << "$$ " << (*it)["id"] << std::endl;
	*/

	std::cout << "~ " << user2.get_main_quests()[0]->get_subquests()[0]->get_id() << std::endl;
	std::cout << "~ " << user2.get_main_quests()[0]->get_subquests()[1]->get_id() << std::endl;

    // ensure the main quest has subquests loaded
    auto main_quest = user2.get_main_quests()[0];

    EXPECT_FALSE(main_quest->get_subquests().empty())
        << "Expected main quest to have subquests, but found none.";

    // check the first subquest
    auto main_subquest1_1 = main_quest->get_subquests().at(0);
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
	auto user = new ServerUser(database, "user_one@email.com", "Test user 1", "the_password_1");

	user->add_main_quests(
		new ServerQuest(database, "id1", user, "caption1", nullptr, {}),
		new ServerQuest(database, "id2", user, "caption2", nullptr, {}),
		new ServerQuest(database, "id3", user, "caption3", nullptr, {})
	);

	auto subquests1 = std::vector<Sidequest::Quest*> {
		new ServerQuest(database, "id1.1", user, "caption1.1", user->get_main_quests()[0], {}),
		new ServerQuest(database, "id1.2", user, "caption1.2", user->get_main_quests()[0], {}),
		new ServerQuest(database, "id1.3", user, "caption1.3", user->get_main_quests()[0], {})
	};
	auto sub_subquests11 = std::vector<Sidequest::Quest*> {
		new ServerQuest(database, "id1.1.1", user, "caption1.1.1", subquests1[0], {}),
		new ServerQuest(database, "id1.1.2", user, "caption1.1.2", subquests1[0], {}),
		new ServerQuest(database, "id1.1.3", user, "caption1.1.3", subquests1[0], {})
	};
	auto sub_subquests12 = std::vector<Sidequest::Quest*> {
		new ServerQuest(database, "id1.2.1", user, "caption1.2.1", subquests1[1], {}),
		new ServerQuest(database, "id1.2.2", user, "caption1.2.2", subquests1[1], {}),
		new ServerQuest(database, "id1.2.3", user, "caption1.2.3", subquests1[1], {})
	};
	auto sub_subquests13 = std::vector<Sidequest::Quest*> {
		new ServerQuest(database, "id1.3.1", user, "caption1.3.1", subquests1[2], {}),
		new ServerQuest(database, "id1.3.2", user, "caption1.3.2", subquests1[2], {}),
		new ServerQuest(database, "id1.3.3", user, "caption1.3.3", subquests1[2], {})
	};

	auto subquests2 = std::vector<Sidequest::Quest*> {
		new ServerQuest(database, "id2.1", user, "caption2.1", user->get_main_quests()[1], {}),
		new ServerQuest(database, "id2.2", user, "caption2.2", user->get_main_quests()[1], {}),
		new ServerQuest(database, "id2.3", user, "caption2.3", user->get_main_quests()[1], {})
	};
	auto sub_subquests21 = std::vector<Sidequest::Quest*> {
		new ServerQuest(database, "id2.1.1", user, "caption2.1.1", subquests2[0], {}),
		new ServerQuest(database, "id2.1.2", user, "caption2.1.2", subquests2[0], {}),
		new ServerQuest(database, "id2.1.3", user, "caption2.1.3", subquests2[0], {})
	};
	auto sub_subquests22 = std::vector<Sidequest::Quest*> {
		new ServerQuest(database, "id2.2.1", user, "caption2.2.1", subquests2[1], {}),
		new ServerQuest(database, "id2.2.2", user, "caption2.2.2", subquests2[1], {}),
		new ServerQuest(database, "id2.2.3", user, "caption2.2.3", subquests2[1], {})
	};
	auto sub_subquests23 = std::vector<Sidequest::Quest*> {
		new ServerQuest(database, "id2.3.1", user, "caption2.3.1", subquests2[2], {}),
		new ServerQuest(database, "id2.3.2", user, "caption2.3.2", subquests2[2], {}),
		new ServerQuest(database, "id2.3.3", user, "caption2.3.3", subquests2[2], {})
	};
	auto subquests3 = std::vector<Sidequest::Quest*> {
		new ServerQuest(database, "id3.1", user, "caption3.1", user->get_main_quests()[2], {}),
		new ServerQuest(database, "id3.2", user, "caption3.2", user->get_main_quests()[2], {}),
		new ServerQuest(database, "id3.3", user, "caption3.3", user->get_main_quests()[2], {})
	};
	auto sub_subquests31 = std::vector<Sidequest::Quest*> {
		new ServerQuest(database, "id3.1.1", user, "caption3.1.1", subquests3[0], {}),
		new ServerQuest(database, "id3.1.2", user, "caption3.1.2", subquests3[0], {}),
		new ServerQuest(database, "id3.1.3", user, "caption3.1.3", subquests3[0], {})
	};
	auto sub_subquests32 = std::vector<Sidequest::Quest*> {
		new ServerQuest(database, "id3.2.1", user, "caption3.2.1", subquests3[1], {}),
		new ServerQuest(database, "id3.2.2", user, "caption3.2.2", subquests3[1], {}),
		new ServerQuest(database, "id3.2.3", user, "caption3.2.3", subquests3[1], {})
	};
	auto sub_subquests33 = std::vector<Sidequest::Quest*> {
		new ServerQuest(database, "id3.3.1", user, "caption3.3.1", subquests3[2], {}),
		new ServerQuest(database, "id3.3.2", user, "caption3.3.2", subquests3[2], {}),
		new ServerQuest(database, "id3.3.3", user, "caption3.3.3", subquests3[2], {})
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
		auto server_quest = ServerQuest(database,
			main_quest->get_id(),
			main_quest->get_owner(),
			main_quest->get_caption(),
			main_quest->get_parent(),
			main_quest->get_subquests());
		// create all main quests on database
		//std::cout << "main quest" << std::endl;

		server_quest.create_on_database();
		for (auto subquest : main_quest->get_subquests()) {
			auto server_subquest = ServerQuest(database,
			subquest->get_id(),
			subquest->get_owner(),
			subquest->get_caption(),
			subquest->get_parent(),
			subquest->get_subquests());
			server_subquest.create_on_database();
			//std::cout << "sub_quest" << std::endl;

			for (auto subsubquest : subquest->get_subquests()) {
				auto server_subsubquest = ServerQuest(database,
				subsubquest->get_id(),
				subsubquest->get_owner(),
				subsubquest->get_caption(),
				subsubquest->get_parent(),
				subsubquest->get_subquests());
				server_subsubquest.create_on_database();
				std::cout << subsubquest->get_id() << std::endl;
			}
			//std::cout << std::endl;
		}
	}

		std::cout << "--check--" << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << "--check--" << std::endl;

/*
		auto q1 = Query(database,
			"SELECT * FROM quest WHERE id like ?;");
		q1.bind(1, "id1%");
		q1.execute();
		for (auto it = q1.begin(); it != q1.end(); ++it) {
			std::cout << (*it)["id"] << std::endl;
		}
		auto q2 = Query(database,
			"SELECT * FROM quest WHERE id like ?;");
		q2.bind(1, "id2%");
		q2.execute();
		for (auto it = q2.begin(); it != q2.end(); ++it) {
			std::cout << (*it)["id"] << std::endl;
		}
		auto q3 = Query(database,
		"SELECT * FROM quest WHERE id like ?;");
		q3.bind(1, "id3%");
		q3.execute();
		for (auto it = q3.begin(); it != q3.end(); ++it) {
			std::cout << (*it)["id"] << std::endl;
		}
*/

			delete user;
			user = nullptr;

			auto user2 = ServerUser(database, "user_one@email.com");
			user2.read_on_database();
			user2.load_main_quests();

			ASSERT_EQ(user2.get_main_quests().size(), 3);
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests().size(), 0);
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests().size(), 0);
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests().size(), 0);


			// load subquests for each main quest
			for (auto main_quest : user2.get_main_quests()) {
				std::cout << "##########" << main_quest->get_id() << "##########" << std::endl;
				auto server_quest = ServerQuest(
					database,
					main_quest->get_id(),
					main_quest->get_owner(),
					main_quest->get_caption(),
					main_quest->get_parent(),
					main_quest->get_subquests());

				server_quest.load_sub_quests();
				main_quest->set_subquests(server_quest.get_subquests());
				std::cout << "##########" << main_quest->get_id() << "##########" << std::endl;
			}


			std::cout << "?? " << user2.get_main_quests()[0]->get_subquests()[0]->get_id() << std::endl;
			std::cout << "?? " << user2.get_main_quests()[0]->get_subquests()[1]->get_id() << std::endl;
			std::cout << "?? " << user2.get_main_quests()[0]->get_subquests()[2]->get_id() << std::endl;

			std::cout << "?? " << user2.get_main_quests()[1]->get_subquests()[0]->get_id() << std::endl;
			std::cout << "?? " << user2.get_main_quests()[1]->get_subquests()[1]->get_id() << std::endl;
			std::cout << "?? " << user2.get_main_quests()[1]->get_subquests()[2]->get_id() << std::endl;

			std::cout << "?? " << user2.get_main_quests()[2]->get_subquests()[0]->get_id() << std::endl;
			std::cout << "?? " << user2.get_main_quests()[2]->get_subquests()[1]->get_id() << std::endl;
			std::cout << "?? " << user2.get_main_quests()[2]->get_subquests()[2]->get_id() << std::endl;


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
						subquest->get_owner(),
						subquest->get_caption(),
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

	ASSERT_EQ(user2.get_main_quests()[0]->get_caption(), "caption1");
	ASSERT_EQ(user2.get_main_quests()[1]->get_caption(), "caption2");
	ASSERT_EQ(user2.get_main_quests()[2]->get_caption(), "caption3");

		ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_id(), "id1.1");
		ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_id(), "id1.2");
		ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_id(), "id1.3");

		ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_caption(), "caption1.1");
		ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_caption(), "caption1.2");
		ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_caption(), "caption1.3");


			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[0]->get_id(), "id1.1.1");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[1]->get_id(), "id1.1.2");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[2]->get_id(), "id1.1.3");

			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[0]->get_caption(), "caption1.1.1");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[1]->get_caption(), "caption1.1.2");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[0]->get_subquests()[2]->get_caption(), "caption1.1.3");

			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[0]->get_id(), "id1.2.1");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[1]->get_id(), "id1.2.2");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[2]->get_id(), "id1.2.3");

			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[0]->get_caption(), "caption1.2.1");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[1]->get_caption(), "caption1.2.2");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[1]->get_subquests()[2]->get_caption(), "caption1.2.3");

			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[0]->get_id(), "id1.3.1");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[1]->get_id(), "id1.3.2");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[2]->get_id(), "id1.3.3");

			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[0]->get_caption(), "caption1.3.1");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[1]->get_caption(), "caption1.3.2");
			ASSERT_EQ(user2.get_main_quests()[0]->get_subquests()[2]->get_subquests()[2]->get_caption(), "caption1.3.3");


		ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_id(), "id2.1");
		ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_id(), "id2.2");
		ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_id(), "id2.3");

		ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_caption(), "caption2.1");
		ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_caption(), "caption2.2");
		ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_caption(), "caption2.3");


			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[0]->get_id(), "id2.1.1");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[1]->get_id(), "id2.1.2");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[2]->get_id(), "id2.1.3");

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[0]->get_caption(), "caption2.1.1");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[1]->get_caption(), "caption2.1.2");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[0]->get_subquests()[2]->get_caption(), "caption2.1.3");

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[0]->get_id(), "id2.2.1");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[1]->get_id(), "id2.2.2");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[2]->get_id(), "id2.2.3");

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[0]->get_caption(), "caption2.2.1");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[1]->get_caption(), "caption2.2.2");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[1]->get_subquests()[2]->get_caption(), "caption2.2.3");

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[0]->get_id(), "id2.3.1");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[1]->get_id(), "id2.3.2");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[2]->get_id(), "id2.3.3");

			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[0]->get_caption(), "caption2.3.1");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[1]->get_caption(), "caption2.3.2");
			ASSERT_EQ(user2.get_main_quests()[1]->get_subquests()[2]->get_subquests()[2]->get_caption(), "caption2.3.3");



		ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_id(), "id3.1");
		ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_id(), "id3.2");
		ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_id(), "id3.3");

		ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_caption(), "caption3.1");
		ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_caption(), "caption3.2");
		ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_caption(), "caption3.3");

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[0]->get_id(), "id3.1.1");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[1]->get_id(), "id3.1.2");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[2]->get_id(), "id3.1.3");

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[0]->get_caption(), "caption3.1.1");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[1]->get_caption(), "caption3.1.2");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[0]->get_subquests()[2]->get_caption(), "caption3.1.3");

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[0]->get_id(), "id3.2.1");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[1]->get_id(), "id3.2.2");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[2]->get_id(), "id3.2.3");

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[0]->get_caption(), "caption3.2.1");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[1]->get_caption(), "caption3.2.2");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[1]->get_subquests()[2]->get_caption(), "caption3.2.3");

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[0]->get_id(), "id3.3.1");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[1]->get_id(), "id3.3.2");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[2]->get_id(), "id3.3.3");

			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[0]->get_caption(), "caption3.3.1");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[1]->get_caption(), "caption3.3.2");
			ASSERT_EQ(user2.get_main_quests()[2]->get_subquests()[2]->get_subquests()[2]->get_caption(), "caption3.3.3");

}
