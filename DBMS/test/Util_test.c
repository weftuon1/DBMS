#include "gtest/gtest.h"
#include "Util.h"
#include "Table.h"

TEST(testUtil, testNewState) {
    State_t *state = new_State();
    ASSERT_NE(state, nullptr);
    ASSERT_EQ(state->saved_stdout, -1);
}

TEST(testUtil, testHandleQueryCmdInsert) {
    Table_t *table = new_Table(NULL);
    char const *args[] = { "insert" };
    Command_t cmd = { QUERY_CMD, (char**)args, 1, 1 };
    int ret;
    ret = handle_query_cmd(table, &cmd);
    ASSERT_EQ(ret, INSERT_CMD);
}

TEST(testUtil, testHandleQueryCmdSelect) {
    Table_t *table = new_Table(NULL);
    char const *args[] = { "select", "*", "from", "table"};
    Command_t cmd = { QUERY_CMD, (char**)args, 4, 4 };
    int ret;
    ret = handle_query_cmd(table, &cmd);
    ASSERT_EQ(ret, SELECT_CMD);
}

TEST(testUtil, testHandleQueryCmdFail) {
    Table_t *table = new_Table(NULL);
    char const *args[] = { "unknown" };
    Command_t cmd = { QUERY_CMD, (char**)args, 1, 1 };
    int ret;
    ret = handle_query_cmd(table, &cmd);
    ASSERT_EQ(ret, UNRECOG_CMD);
}

TEST(testUtil, testHandleInsertCmd) {
    Table_t *table = new_Table(NULL);
    char const *args[] = { "insert", "1", "user1", \
            "user1@example.com", "21" };
    Command_t cmd = { QUERY_CMD, (char **)args, 5, 5 };
    int ret;
    ret = handle_insert_cmd(table, &cmd);
    ASSERT_EQ(ret, 1);
    ASSERT_EQ(cmd.type, INSERT_CMD);
    ASSERT_EQ(table->len, 1);
}

TEST(testUtil, testHandleSelectCmdEmpty) {
    Table_t *table = new_Table(NULL);
    char const *args[] = { "select", "*", "from", "table"};
    Command_t cmd = { QUERY_CMD, (char**)args, 4, 4 };
    int ret;

    ret = handle_select_cmd(table, &cmd);
    ASSERT_EQ(ret, table->len);
}

TEST(testUtil, testHandleSelectCmd) {
    Table_t *table = new_Table(NULL);
    char const *args[] = { "select", "*", "from", "table"};
    Command_t cmd = { QUERY_CMD, (char**)args, 4, 4 };
    User_t user = { 1, "user", "user@example.com", 20 };
    int ret;
    size_t idx;

    testing::internal::CaptureStdout();
    for (idx = 0; idx < 500; idx++) {
        cmd.type = QUERY_CMD;
        add_User(table, &user);
        ret = handle_select_cmd(table, &cmd);
        ASSERT_EQ(ret, table->len);
        ASSERT_EQ(cmd.type, SELECT_CMD);
    }
    testing::internal::GetCapturedStdout();
}

