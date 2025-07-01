//
// Created by roger on 6/30/25.
//
#include <gtest/gtest.h>

#include <iostream>

/*
#include "network/serializable_user.h"

using namespace Sidequest;

class SerialiseTestsUser : public ::testing::Test
{
protected:

    SerialiseTestsUser()
    {
    }

    virtual ~SerialiseTestsUser() {
    }

    SerialisableUser* createSerializeDeserializeUser(Id id, std::string email, std::string display_name, std::string password)
    {
        auto user = new SerialisableUser("unitest@hs-aalende", "Unit Test User", "secret");
        user->id = 12;
        auto json = user->to_json();
        delete(user);

        std::string json_string = json.dump(4);
        std::cout << json_string << std::endl;

        auto user2 = new SerialisableUser(0);

        auto json2 = Json::parse(json_string);
        user2->from_json(json2);
        return user2;
    }
};

TEST_F(SerialiseTestsUser, SERIALIZE_USER)
{
    auto user = createSerializeDeserializeUser(12, "unitest@hs-aalende", "Unit Test User", "1234" );

    ASSERT_EQ(user->id, 12);
    ASSERT_EQ(user->email, "unitest@hs-aalende");
    ASSERT_EQ(user->display_name, "Unit Test User");

    delete(user);
}

TEST_F(SerialiseTestsUser, SERIALIZE_USER_NO_PASSWORD)
{
    auto user = createSerializeDeserializeUser(12, "unitest@hs-aalende", "Unit Test User", "1234");

    ASSERT_EQ(user->password, "");

    delete(user);
}
*/