//
// Created by roger on 6/30/25.
//
#include <gtest/gtest.h>

#include <iostream>

#include "network/serializable_quest.h"
#include "network/serializable_user.h"

using namespace Sidequest;

class SerialiseTestsQuest : public ::testing::Test {
 protected:
	SerialiseTestsQuest() {}

	virtual ~SerialiseTestsQuest() {}


	SerializableQuest* serialize_then_deserialize(SerializableQuest* original_quest, JsonSerializable::SerializationMode mode = JsonSerializable::one_level) {

		//std::cout << "SerialiseTestsQuest::serialize_then_deserialize()" << std::endl;

		auto json = original_quest->to_json(mode);
		std::cout << "Before segfault" << std::endl;

		std::string json_string = json.dump(4);
		std::cout << json_string << std::endl;

		auto new_quest = new SerializableQuest("id1");

		auto json2 = Json::parse(json_string);


		new_quest->from_json(json2);
		return new_quest;
	}
};

TEST_F(SerialiseTestsQuest, SERIALIZE_QUEST) {
	auto quest = new SerializableQuest("id1", Quest::Status::done, "Unit Test Quest", "Caption\ncan be several lines\nor not.", nullptr, nullptr, nullptr);

	//std::cout << quest->to_json().dump(4) << std::endl;

	auto new_quest = serialize_then_deserialize(quest, JsonSerializable::all_levels);
	std::cout << "Before segfault" << std::endl;

	ASSERT_EQ(quest->get_id(), "id1");
	ASSERT_EQ(quest->get_status(), Quest::Status::done);
	ASSERT_EQ(quest->get_title(), "Unit Test Quest");
	ASSERT_EQ(quest->get_caption(), "Caption\ncan be several lines\nor not.");

	ASSERT_EQ(quest->get_owner(), nullptr);
	ASSERT_EQ(quest->get_editor(), nullptr);
	ASSERT_EQ(quest->get_parent(), nullptr);

	delete(quest);
	delete(new_quest);
}


TEST_F(SerialiseTestsQuest, SERIALIZE_QUEST_PARENT) {
	auto quest = new SerializableQuest("id_of_child", Quest::Status::done, "child quest", "Description\ncan be several lines\nor not.", nullptr, nullptr, nullptr);
	auto parent_quest = new SerializableQuest("id_of_parent", Quest::Status::done, "parent quest", "c", nullptr, nullptr, nullptr);
	quest->set_parent(parent_quest);


	//quest->serialize_parent_recursive = true;

	// falsch, id sollte bei no levels oder one_level stehen stehen aber nicht bei all_levels
	//quest->parent_recursive_mode = JsonSerializable::all_levels;
	std::cout <<  "before null error" << std::endl;

	nlohmann::json json_string = quest->to_json(JsonSerializable::one_level);
	std::cout <<  json_string.dump(4) << std::endl;


	SerializableQuest* q_new = new SerializableQuest("id_of_child");
	q_new->from_json(json_string);

	auto new_quest = serialize_then_deserialize(quest, JsonSerializable::one_level);
	delete(quest);
	delete(parent_quest);

	ASSERT_EQ(new_quest->get_id(), "id_of_child");
	ASSERT_EQ(new_quest->get_title(), "child quest");
	ASSERT_EQ(new_quest->get_caption(), "Description\ncan be several lines\nor not.");
	ASSERT_EQ(new_quest->get_status(), Quest::Status::done);
	ASSERT_EQ(new_quest->get_owner(), nullptr);
	ASSERT_EQ(new_quest->get_editor(), nullptr);
	ASSERT_EQ(new_quest->get_parent()->get_id(), "id_of_parent");
	ASSERT_EQ(new_quest->get_parent()->get_title(), "parent quest");

	delete(new_quest);

}

TEST_F(SerialiseTestsQuest, SERIALIZE_QUEST_OWNER_EDITOR) {
	auto quest = new SerializableQuest("id1", Quest::Status::done, "Unit Test Quest", "caption\ncan be several lines\nor not.", nullptr, nullptr, nullptr);

	auto owner = new SerializableUser("unitest1@hs-aalen.de", "Unit Test User 1", "secret");
	quest->set_owner(owner);

	auto editor = new SerializableUser("unitest2@hs-aalen.de", "Unit Test User 2", "secret");
	quest->set_editor(editor);
	//quest->editor->get_email() = quest->editor->id;

	//quest->serialize_owner_recursive = true;
	//quest->serialize_editor_recursive = true;

	auto new_quest = serialize_then_deserialize(quest, JsonSerializable::all_levels);
	delete(quest);
	delete(owner);
	delete(editor);

	ASSERT_EQ(new_quest->get_id(), "id1");
	ASSERT_EQ(new_quest->get_title(), "Unit Test Quest");
	ASSERT_EQ(new_quest->get_caption(), "Caption\ncan be several lines\nor not.");
	ASSERT_EQ(new_quest->get_status(), Quest::Status::done);
	ASSERT_EQ(new_quest->get_owner()->get_email(), "unitest1@hs-aalen.de");
	ASSERT_EQ(new_quest->get_editor()->get_id(), "unitest2@hs-aalen.de");

	delete(new_quest);
}
