#include <gtest/gtest.h>
#include <filesystem>

#include "model/server_quest.h"
#include "storage/database.h"
#include "storage/database_exceptions.h"
#include "model/server_user.h"

class CRUDTests : public ::testing::Test
{
protected:
	Sidequest::Server::Database* database;

	CRUDTests()
	{
	}

	virtual ~CRUDTests() {
	}

	virtual void SetUp() {
		database = new Sidequest::Server::Database(":memory:");
	}

	virtual void TearDown() {
		delete database;
	}
};

using namespace Sidequest::Server;

TEST_F(CRUDTests, OPEN_DATABASE) {
}

TEST_F(CRUDTests, CRUD_USER_CREATE) {
	auto user = new ServerUser( database, "crud_user_create@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	delete(user);
	user = nullptr;

	auto user2 = new ServerUser(database, "crud_user_create@hs-aalen.de");
	user2->read_on_database();

	EXPECT_EQ(user2->get_display_name(), "Temporary User");
	delete(user2);
}

TEST_F(CRUDTests, CRUD_USER_CREATE_DOUBLE) {
	auto user = new ServerUser(database, "crud_user_create_double@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	delete(user);
	user = nullptr;

	try {
		auto user = new ServerUser(database, "crud_user_create_double@hs-aalen.de", "Temporary User 2", "");
		user->create_on_database();
		FAIL();
	}
	catch (const UnableToCreateObjectException& expected)
	{
		delete(user);
		user = nullptr;
	}
}

TEST_F(CRUDTests, CRUD_USER_READ) {
	auto user = new ServerUser(database, "crud_user_read@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	delete(user);

	user = new ServerUser(database, "crud_user_read@hs-aalen.de");
	user->read_on_database();

	EXPECT_EQ(user->get_display_name(), "Temporary User");
}

TEST_F(CRUDTests, CRUD_USER_UPDATE) {
	auto user = new ServerUser(database, "crud_user_update@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	user->set_display_name("Changed Display Name");
	user->update_on_database();
	delete(user);

	auto user2 = new ServerUser(database, "crud_user_update@hs-aalen.de");
	user2->read_on_database();

	EXPECT_EQ(user->get_display_name(), "Changed Display Name");
	delete(user2);
}

TEST_F(CRUDTests, CRUD_USER_DELETE) {
	auto user = new ServerUser(database, "crud_user_delete@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	delete(user);
	user = nullptr;

	auto user2 = new ServerUser(database, "crud_user_delete@hs-aalen.de");
	user2->delete_on_database();
	delete(user2);

	try {
		auto user3 = new ServerUser(database, "crud_user_delete@hs-aalen.de");
		user3->read_on_database();
		FAIL();
	}
	catch (const UnableToReadObjectException& expected)
	{
	}
}

TEST_F(CRUDTests, CRUD_QUEST_CREATE) {

	auto owner = new ServerUser(database, "test_user_mail");
	auto quest1 = new ServerQuest(database, "id1", owner, "caption1", nullptr, std::vector<Sidequest::Quest*>() );
	quest1->create_on_database();

	delete(quest1);
	quest1 = nullptr;
	delete owner;
	owner = nullptr;

	auto quest2 = new ServerQuest(database, "id1");
	quest2->read_on_database();

	EXPECT_EQ(quest2->get_caption(), "caption1");
	EXPECT_EQ(quest2->get_owner()->get_email(), "test_user_mail");
	delete(quest2);
	quest2 = nullptr;

}


TEST_F(CRUDTests, CRUD_QUEST_CREATE_DOUBLE) {
	auto owner1 = new ServerUser(database, "test_user1_mail");
	auto quest1 = new ServerQuest(database, "id1", owner1, "caption1", nullptr, std::vector<Sidequest::Quest*>() );
	quest1->create_on_database();
	delete(quest1);
	try {
		auto quest1 = new ServerQuest(database, "id1", owner1, "caption2", nullptr, std::vector<Sidequest::Quest*>() );
		quest1->create_on_database();
		FAIL();
	}
	catch (const UnableToCreateObjectException& expected)
	{
		delete(quest1);
	}
	delete(owner1);
}

TEST_F(CRUDTests, CRUD_QUEST_READ) {
	auto owner = new ServerUser(database, "test_user_mail");
	auto quest = new ServerQuest(database, "id1", owner, "caption3", nullptr, std::vector<Sidequest::Quest*>() );
	quest->create_on_database();
	delete(quest);
	quest = nullptr;

	quest = new ServerQuest(database, "id1");
	quest->read_on_database();

	EXPECT_EQ(quest->get_caption(), "caption3");
	EXPECT_EQ(quest->get_owner()->get_email(), "test_user_mail");
}

TEST_F(CRUDTests, CRUD_QUEST_UPDATE) {
	auto owner = new ServerUser(database, "test_user_mail");
	auto quest = new ServerQuest(database, "id1", owner, "caption3", nullptr, std::vector<Sidequest::Quest*>() );
	quest->create_on_database();
	delete(owner);
	owner = nullptr;
	auto owner2 = new ServerUser(database, "test_user_mail_update");
	quest->set_caption("new caption");
	quest->set_owner(owner2);
	quest->update_on_database();
	delete(quest);
	quest = nullptr;

	quest = new ServerQuest(database, "id1");
	quest->read_on_database();

	EXPECT_EQ(quest->get_caption(), "new caption");
	EXPECT_EQ(quest->get_owner()->get_email(), "test_user_mail_update");

	delete(quest);
	delete(owner2);
	owner2 = nullptr;
}

TEST_F(CRUDTests, CRUD_QUEST_DELETE) {
	auto owner = new ServerUser(database, "test_owner_mail@email.com");
	auto quest = new ServerQuest(database, "id1", owner, "the caption.", nullptr, std::vector<Sidequest::Quest*>());
	quest->create_on_database();
	delete(owner);
	owner = nullptr;

	quest->delete_on_database();
	delete(quest);
	quest = nullptr;

	try {
		auto read_quest = new ServerQuest(database, "id1");
		read_quest->read_on_database();
		delete(read_quest);
		read_quest = nullptr;
		FAIL();
	} catch (const UnableToReadObjectException& expected) {
	}

}


