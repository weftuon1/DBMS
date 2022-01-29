#include <stdio.h>
#include <sys/stat.h>
#include "gtest/gtest.h"
#include "Table.h"
#include "User.h"
#include "test_Util.h"

TEST(testUniqueID, testNoFile) {
    User_t user = { 1, "user1", "user1@example.com", 20 };
    Table_t *table = new_Table(NULL);
    int ret;

    ret = add_User(table, &user);
    ASSERT_EQ(ret, 1);

    ret = add_User(table, &user);
    ASSERT_NE(ret, 1);
}

TEST(testUniqueID, testFile) {
    char file_name[] = "./test/test.db";
    User_t user = { 1, "user1", "user1@example.com", 20 };
    Table_t *table;
    int ret;
    struct stat st;

    if (stat(file_name, &st) == 0) {
        remove(file_name);
        ASSERT_NE(stat(file_name, &st), 0);
    }

    table = new_Table(file_name);

    ret = add_User(table, &user);
    ASSERT_EQ(ret, 1);

    ret = add_User(table, &user);
    ASSERT_NE(ret, 1);
}

TEST(testUniqueID, testFileWithArchivedFile) {
    char file_name[] = "./test/test.db";
    User_t user = { 1, "user1", "user1@example.com", 20 };
    Table_t *table;
    int ret;

    setup_sample_db_file(file_name, 50);

    table = new_Table(file_name);

    ret = add_User(table, &user);
    ASSERT_NE(ret, 1);
}
