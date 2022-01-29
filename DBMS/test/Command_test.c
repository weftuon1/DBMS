#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "Command.h"
#include "gtest/gtest.h"

TEST(testCommand, testNewCommand) {
    Command_t *cmd = new_Command();
    ASSERT_EQ(cmd->type, UNRECOG_CMD);
    ASSERT_EQ(cmd->args, nullptr);
    ASSERT_TRUE(cmd->args_len == 0);
    ASSERT_TRUE(cmd->args_cap == 0);
}


TEST(testCommand, testAddArg) {
    Command_t *cmd = new_Command();
    char const *args[] = { "test1", "test2", "test3", \
                            "test4", "test5", "test6" };
    size_t idx;
    int ret = add_Arg(cmd, args[0]);
    if (ret == 0) {
        ASSERT_NE(cmd->args, nullptr);
        ASSERT_TRUE(!strncmp(cmd->args[0], "test1", 5));
        ASSERT_TRUE(cmd->args_len == 1);
        ASSERT_TRUE(cmd->args_cap == 5);

        for (idx = 1; idx < 6; idx++) {
            add_Arg(cmd, args[idx]);
        }

        ASSERT_NE(cmd->args, nullptr);
        ASSERT_EQ(strncmp(cmd->args[5], "test6", 5), 0);
        ASSERT_TRUE(cmd->args_len == 6);
        ASSERT_TRUE(cmd->args_cap == 10);
    } else {
        ASSERT_EQ(cmd->args, nullptr);
    }
}

TEST(testCommand, testAddSelectArg) {
    Command_t *cmd = new_Command();
    cmd->type = SELECT_CMD;
    char const *fields[] = { "*", "id", "email", "age", \
                             "name", "email" };
    size_t fields_len = 6;
    size_t idx;
    for (idx = 0; idx < fields_len; idx++) {
        add_select_field(cmd, fields[idx]);
        ASSERT_EQ(cmd->cmd_args.sel_args.fields_len, idx+1);
        ASSERT_STREQ(cmd->cmd_args.sel_args.fields[idx], fields[idx]);
    }
}

