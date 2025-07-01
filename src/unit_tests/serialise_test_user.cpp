//
// Created by roger on 6/30/25.
//
#include <gtest/gtest.h>

#include <iostream>

#include "network/serializable_user.h"

using namespace Sidequest;

class SerialiseTestsUser : public ::testing::Test {
protected:

    SerialiseTestsUser() {}

    virtual ~SerialiseTestsUser() {}

    SerializableUser* createSerializeDeserializeUser(Id email, std::string display_name, std::string password) {
        auto user = new SerializableUser("unitest@hs-aalen.de", "Unit Test User", "secret");
        auto json = user->to_json();
        std::cout << "Input JSON: " << json.dump(4) << std::endl;

        delete(user);

        std::string json_string = json.dump(4);
        //std::cout << json_string << std::endl;

        auto user2 = new SerializableUser("unitest@hs-aalen.de");

        auto json2 = Json::parse(json_string);
        user2->from_json(json2);
        return user2;
    }
};

TEST_F(SerialiseTestsUser, SERIALIZE_USER) {
    auto user = createSerializeDeserializeUser("unitest@hs-aalen.de", "Unit Test User", "1234" );

    std::cout << user->get_id() << std::endl;
    ASSERT_EQ(user->get_id(), "unitest@hs-aalen.de");
    ASSERT_EQ(user->get_display_name(), "Unit Test User");

    delete(user);
}

TEST_F(SerialiseTestsUser, SERIALIZE_USER_NO_PASSWORD) {
    auto user = createSerializeDeserializeUser("unitest@hs-aalen.de", "Unit Test User", "1234");

    ASSERT_EQ(user->get_password(), "");

    delete(user);
}