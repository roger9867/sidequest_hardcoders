//
// Created by roger on 6/30/25.
//
/*
#include <gtest/gtest.h>

#include <iostream>

#include "network/serializable_quest.h"
#include "network/serializable_user.h"

using namespace Sidequest;

class SerialiseTestsQuest : public ::testing::Test
{
protected:

	SerialiseTestsQuest()
	{
	}

	virtual ~SerialiseTestsQuest() {
	}

	SerialisableQuest* serialize_then_deserialize(SerialisableQuest* original_quest)
	{
		auto json = original_quest->to_json();

		std::string json_string = json.dump(4);
		std::cout << json_string << std::endl;

		auto new_quest = new SerialisableQuest(0);

		auto json2 = Json::parse(json_string);
		new_quest->from_json(json2);
		return new_quest;
	}

};

TEST_F(SerialiseTestsQuest, SERIALIZE_QUEST)
{
	auto quest = new SerialisableQuest( Quest::Status::done, "Unit Test Quest", "Description\ncan be several lines\nor not.", nullptr, nullptr, nullptr );
	quest->id = 12;
	auto new_quest = serialize_then_deserialize( quest );

	ASSERT_EQ(quest->id, 12);
	ASSERT_EQ(quest->title, "Unit Test Quest");
	ASSERT_EQ(quest->description, "Description\ncan be several lines\nor not.");
	ASSERT_EQ(quest->status, Quest::Status::done);
	ASSERT_EQ(quest->owner, nullptr);
	ASSERT_EQ(quest->editor, nullptr);
	ASSERT_EQ(quest->parent, nullptr);

	delete(quest);
	delete(new_quest);
}

TEST_F(SerialiseTestsQuest, SERIALIZE_QUEST_PARENT)
{
	auto quest = new SerialisableQuest(Quest::Status::done, "Unit Test Quest", "Description\ncan be several lines\nor not.", nullptr, nullptr, nullptr);
	quest->id = 12;
	auto parent_quest = new SerialisableQuest(Quest::Status::done, "Unit Test Main Quest", "", nullptr, nullptr, nullptr);
	quest->parent = parent_quest;
	quest->parent->id = 42;
	quest->parent_id = 42;
	quest->serialise_parent_recursive = true;
	auto new_quest = serialize_then_deserialize(quest);
	delete(quest);
	delete(parent_quest);

	ASSERT_EQ(new_quest->id, 12);
	ASSERT_EQ(new_quest->title, "Unit Test Quest");
	ASSERT_EQ(new_quest->description, "Description\ncan be several lines\nor not.");
	ASSERT_EQ(new_quest->status, Quest::Status::done);
	ASSERT_EQ(new_quest->owner, nullptr);
	ASSERT_EQ(new_quest->editor, nullptr);
	ASSERT_EQ(new_quest->parent->id, 42);
	ASSERT_EQ(new_quest->parent_id, 42);
	ASSERT_EQ(new_quest->parent->title, "Unit Test Main Quest");

	delete(new_quest);
}

TEST_F(SerialiseTestsQuest, SERIALIZE_QUEST_OWNER_EDITOR)
{
	auto quest = new SerializableQuest(Quest::Status::done, "Unit Test Quest", "Description\ncan be several lines\nor not.", nullptr, nullptr, nullptr);
	quest->id = 12;

	auto owner = new SerializableUser("unitest1@hs-aalende", "Unit Test User 1", "secret");
	owner->id = 101;
	quest->owner = owner;
	quest->owner_id = quest->owner->id;

	auto editor = new SerializableUser("unitest2@hs-aalende", "Unit Test User 2", "secret");
	editor->id = 102;
	quest->editor = editor;
	quest->editor->get_email() = quest->editor->id;

	quest->serialize_owner_recursive = true;
	quest->serialise_editor_recursive = true;

	auto new_quest = serialize_then_deserialize(quest);
	delete(quest);
	delete(owner);
	delete(editor);

	ASSERT_EQ(new_quest->id, 12);
	ASSERT_EQ(new_quest->title, "Unit Test Quest");
	ASSERT_EQ(new_quest->description, "Description\ncan be several lines\nor not.");
	ASSERT_EQ(new_quest->status, Quest::Status::done);
	ASSERT_EQ(new_quest->owner->id, 101);
	ASSERT_EQ(new_quest->editor->id, 102);
	ASSERT_EQ(new_quest->owner->email, "unitest1@hs-aalende");

	delete(new_quest);
}
*/