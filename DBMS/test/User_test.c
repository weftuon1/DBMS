#include "gtest/gtest.h"
#include "User.h"
#include "Command.h"

TEST(testUser, testNewUser) {
    User_t *user = new_User();
    ASSERT_NE(user, nullptr);
    ASSERT_EQ(user->id, 0);
    ASSERT_EQ(strlen(user->name), 0);
    ASSERT_EQ(strlen(user->email), 0);
    ASSERT_EQ(user->age, 0);
}

TEST(testUser, testCommandToUserSuc) {
    char const *args[] = { "insert", "1", "user1", "user1@example.com", "20" };
    Command_t cmd = { QUERY_CMD, (char **)args, 5, 5 };
    User_t *user;
    user = command_to_User(&cmd);
    ASSERT_NE(user, nullptr);
    ASSERT_EQ(user->id, atoi(args[1]));
    ASSERT_STREQ(user->name, args[2]);
    ASSERT_STREQ(user->email, args[3]);
    ASSERT_EQ(user->age, atoi(args[4]));
    free(user);
}

TEST(testUser, testCommandToUserFail) {
    Command_t *cmd;
    User_t *user;

    cmd = NULL;
    user = command_to_User(cmd);
    ASSERT_EQ(user, nullptr);
}

