#include <stdio.h>
#include <sys/stat.h>
#include "gtest/gtest.h"
#include "Table.h"
#include "User.h"
#include "test_Util.h"

TEST(testTable, testNewTable) {
    Table_t *table = new_Table(NULL);
    size_t idx;
    ASSERT_NE(table, nullptr);
    ASSERT_EQ(table->capacity, INIT_TABLE_SIZE);
    ASSERT_EQ(table->len, 0);
    ASSERT_NE(table->users, nullptr);
    ASSERT_NE(table->cache_map, nullptr);
    for (idx = 0; idx < INIT_TABLE_SIZE; idx++) {
        ASSERT_FALSE(table->cache_map[idx]);
    }
    ASSERT_EQ(table->fp, nullptr);
}

TEST(testTable, testCacheMapMem) {
    Table_t *table = new_Table(NULL);
    User_t user = { 1, "user", "user@example.com", 20 };
    size_t idx;
    int ret = 0;
    const size_t insert_count = 50;
    for (idx = 0; idx < insert_count; idx++) {
        setup_sample_user(&user, idx);
        ret = add_User(table, &user);
        ASSERT_EQ(ret, 1);
        ASSERT_EQ(table->len, idx+1);
        ASSERT_TRUE(table->len <= table->capacity);
    }
    for (idx = 0; idx < insert_count; idx++) {
        ASSERT_TRUE(table->cache_map[idx]);
    }
}

TEST(testTable, testCacheMap) {
    char file_name[] = "./test/test.db";
    Table_t *table;
    User_t user = { 1, "user", "user@example.com", 20 };
    size_t idx;
    int ret = 0;
    struct stat st;
    const size_t insert_count = 50;

    if (stat(file_name, &st) == 0) {
        remove(file_name);
        ASSERT_NE(stat(file_name, &st), 0);
    }
    table = new_Table(file_name);

    for (idx = 0; idx < insert_count; idx++) {
        setup_sample_user(&user, idx);
        ret = add_User(table, &user);
        ASSERT_EQ(ret, 1);
        ASSERT_EQ(table->len, idx+1);
        ASSERT_TRUE(table->len <= table->capacity);
    }
    for (idx = 0; idx < insert_count; idx++) {
        ASSERT_TRUE(table->cache_map[idx]);
    }
    if (table->fp) {
        fclose(table->fp);
    }
    remove(file_name);
    ASSERT_NE(stat(file_name, &st), 0);
}

TEST(testTable, testCacheMapWithArchiveFile) {
    char file_name[] = "./test/test.db";
    Table_t *table;
    User_t *tmp_user;
    User_t user = { 1, "user", "user@example.com", 20 };
    size_t idx;
    int ret = 0;
    const size_t insert_count = 50;
    struct stat st;

    setup_sample_db_file(file_name, 2);

    table = new_Table(file_name);

    ASSERT_FALSE(table->cache_map[0]);
    ASSERT_FALSE(table->cache_map[1]);

    for (idx = 0; idx < insert_count; idx++) {
        setup_sample_user(&user, idx+2);
        ret = add_User(table, &user);
        ASSERT_EQ(ret, 1);
    }
    for (idx = 2; idx < insert_count+2; idx++) {
        ASSERT_TRUE(table->cache_map[idx]);
    }

    check_sample_table_record_match(table, 2);

    tmp_user = get_User(table, 2);
    ASSERT_NE(tmp_user, nullptr);
    for (idx = 2; idx < insert_count+2; idx++) {
        ASSERT_TRUE(table->cache_map[idx]);
    }

    if (table->fp) {
        fclose(table->fp);
    }
    remove(file_name);
    ASSERT_NE(stat(file_name, &st), 0);
}

TEST(testTable, testNewTableWithFile) {
    char file_name[] = "./test/test.db";
    Table_t *table = new_Table(file_name);
    struct stat st;

    ASSERT_NE(table, nullptr);
    ASSERT_EQ(table->capacity, INIT_TABLE_SIZE);
    ASSERT_EQ(table->len, 0);
    ASSERT_NE(table->users, nullptr);
    ASSERT_NE(table->fp, nullptr);
    ASSERT_STREQ(table->file_name, file_name);
    fclose(table->fp);
    remove(file_name);
    ASSERT_NE(stat(file_name, &st), 0);
}

TEST(testTable, testNewTableWithOldFile) {
    char file_name[] = "./test/test.db";
    Table_t *table;
    struct stat st;

    setup_sample_db_file(file_name, 2);
    {
        struct stat st;
        ASSERT_EQ(stat(file_name, &st), 0);
        ASSERT_EQ(st.st_size, sizeof(User_t)*2);
    }

    table = new_Table(file_name);
    ASSERT_NE(table, nullptr);
    ASSERT_EQ(table->capacity, INIT_TABLE_SIZE);
    ASSERT_EQ(table->len, 2);
    ASSERT_NE(table->users, nullptr);
    ASSERT_NE(table->fp, nullptr);
    ASSERT_STREQ(table->file_name, file_name);
    fclose(table->fp);
    remove(file_name);
    ASSERT_NE(stat(file_name, &st), 0);
}

TEST(testTable, testNewTableWithOldFileLarge) {
    char file_name[] = "./test/test.db";
    Table_t *table;
    struct stat st;

    setup_sample_db_file(file_name, INIT_TABLE_SIZE+2);

    table = new_Table(file_name);
    ASSERT_NE(table, nullptr);
    ASSERT_EQ(table->len, INIT_TABLE_SIZE+2);
    ASSERT_TRUE(table->len <= table->capacity);
    ASSERT_NE(table->users, nullptr);
    ASSERT_NE(table->fp, nullptr);
    ASSERT_STREQ(table->file_name, file_name);
    fclose(table->fp);
    remove(file_name);
    ASSERT_NE(stat(file_name, &st), 0);
}

TEST(testTable, testAddUserSuc) {
    Table_t *table = new_Table(NULL);
    ASSERT_NE(table, nullptr);

    User_t user;
    setup_sample_user(&user, 0);

    ASSERT_EQ(add_User(table, &user), 1);
    ASSERT_TRUE(table->len <= table->capacity);
    ASSERT_EQ(table->len, 1);
    check_sample_table_record_match(table, 1);
}

TEST(testTable, testAddUserFail) {
    Table_t *table = new_Table(NULL);
    User_t user = { 1, "First User", "first@example.com", 21 };
    ASSERT_NE(add_User(NULL, NULL), 1);
    ASSERT_NE(add_User(table, NULL), 1);
    ASSERT_NE(add_User(NULL, &user), 1);
}

TEST(testTable, testAddUserNoFull) {
    Table_t *table = new_Table(NULL);
    User_t user = { 1, "user", "user@example.com", 20 };
    size_t idx;
    int ret = 0;
    for (idx = 0; idx < INIT_TABLE_SIZE*2; idx++) {
        setup_sample_user(&user, idx);
        ret = add_User(table, &user);
        ASSERT_EQ(ret, 1);
        ASSERT_EQ(table->len, idx+1);
        ASSERT_TRUE(table->len <= table->capacity);
    }
}

TEST(testTable, testArchiveTable) {
    char file_name[] = "./test/test.db";
    Table_t *table;
    struct stat st;
    User_t user = { 1, "user", "user@example.com", 20 };
    const size_t insert_count = 5;
    size_t idx;
    int ret;

    if (stat(file_name, &st) == 0) {
        remove(file_name);
        ASSERT_NE(stat(file_name, &st), 0);
    }

    table = new_Table(file_name);
    for (idx = 0; idx < insert_count; idx++) {
        setup_sample_user(&user, idx);
        ret = add_User(table, &user);
        EXPECT_EQ(ret, 1);
        EXPECT_EQ(table->len, idx+1);
    }
    ret = archive_table(table);
    EXPECT_EQ(ret, insert_count);
    EXPECT_EQ(stat(file_name, &st), 0);
    EXPECT_EQ(st.st_size, sizeof(User_t)*insert_count);

    if (table->fp) {
        fclose(table->fp);
    }
    remove(file_name);
    ASSERT_NE(stat(file_name, &st), 0);
}

TEST(testTable, testArchiveOldTable) {
    char file_name[] = "./test/test.db";
    Table_t *table;
    struct stat st;
    User_t user = { 3, "user", "user@example.com", 20 };
    const size_t insert_count = 5;
    size_t idx;
    int ret;

    setup_sample_db_file(file_name, 2);

    table = new_Table(file_name);
    for (idx = 0; idx < insert_count; idx++) {
        setup_sample_user(&user, idx+2);
        ret = add_User(table, &user);
        EXPECT_EQ(ret, 1);
    }
    ret = archive_table(table);
    EXPECT_EQ(ret, (int)insert_count+2);
    EXPECT_EQ(stat(file_name, &st), 0);
    EXPECT_EQ(st.st_size, sizeof(User_t)*(insert_count+2));

    if (table->fp) {
        fclose(table->fp);
    }
    remove(file_name);
    ASSERT_NE(stat(file_name, &st), 0);
}

TEST(testTable, testArchiveTableNULL) {
    Table_t *table = new_Table(NULL);
    int ret;
    ASSERT_NE(table, nullptr);
    ASSERT_EQ(table->capacity, INIT_TABLE_SIZE);
    ASSERT_EQ(table->len, 0);
    ASSERT_NE(table->users, nullptr);
    ASSERT_EQ(table->fp, nullptr);

    ret = archive_table(table);
    ASSERT_EQ(ret, 0);
}

TEST(testTable, testGetUserNoFile) {
    Table_t *table;
    User_t sample_user;
    size_t idx;
    int ret;

    table = new_Table(NULL);
    for (idx = 0; idx < 2; idx++) {
        setup_sample_user(&sample_user, idx);
        ret = add_User(table, &sample_user);
        EXPECT_EQ(ret, 1);
        EXPECT_EQ(table->len, idx+1);
    }

    check_sample_table_record_match(table, 2);
}

TEST(testTable, testGetUserBeforeArchive) {
    char file_name[] = "./test/test.db";
    Table_t *table;
    struct stat st;
    User_t sample_user;
    size_t idx;
    int ret;

    if (stat(file_name, &st) == 0) {
        remove(file_name);
        ASSERT_NE(stat(file_name, &st), 0);
    }

    table = new_Table(file_name);
    for (idx = 0; idx < 2; idx++) {
        setup_sample_user(&sample_user, idx);
        ret = add_User(table, &sample_user);
        EXPECT_EQ(ret, 1);
        EXPECT_EQ(table->len, idx+1);
    }

    check_sample_table_record_match(table, 2);

    if (table->fp) {
        fclose(table->fp);
    }
    remove(file_name);
    ASSERT_NE(stat(file_name, &st), 0);
}

TEST(testTable, testGetUserFile) {
    char file_name[] = "./test/test.db";
    Table_t *table;
    struct stat st;
    int ret;
    User_t *tmp_user;
    User_t sample_user;
    int idx;

    setup_sample_db_file(file_name, 2);

    table = new_Table(file_name);
    setup_sample_user(&sample_user, 2);
    ret = add_User(table, &sample_user);
    EXPECT_EQ(ret, 1);

    for (idx = 0; idx < 3; idx++) {
        tmp_user = get_User(table, idx);
        setup_sample_user(&sample_user, idx);
        ASSERT_NE(tmp_user, nullptr);
        EXPECT_EQ(tmp_user->id, sample_user.id);
        EXPECT_STREQ(tmp_user->name, sample_user.name);
        EXPECT_STREQ(tmp_user->email, sample_user.email);
        EXPECT_EQ(tmp_user->age, sample_user.age);
    }

    if (table->fp) {
        fclose(table->fp);
    }
    remove(file_name);
    ASSERT_NE(stat(file_name, &st), 0);
}

TEST(testTable, testLoadTableEmpty) {
    char file_name[] = "./test/test.db";
    Table_t *table;
    int idx, ret;

    setup_sample_db_file(file_name, 2);

    table = new_Table(NULL);

    ret = load_table(table, file_name);

    ASSERT_EQ(ret, table->len);
    ASSERT_EQ(ret, 2);
    ASSERT_NE(table->fp, nullptr);
    ASSERT_STREQ(table->file_name, file_name);
    ASSERT_EQ(table->len, 2);
    for (idx = 0; idx < 2; idx++) {
        ASSERT_FALSE(table->cache_map[idx]);
    }
}

TEST(testTable, testLoadTable) {
    char file_name[] = "./test/test.db";
    Table_t *table;
    int idx, ret;
    User_t sample_user;
    const int insert_count = 50;

    setup_sample_db_file(file_name, 2);

    table = new_Table(NULL);
    for (idx = 0; idx < insert_count; idx++) {
        setup_sample_user(&sample_user, idx);
        add_User(table, &sample_user);
    }

    ret = load_table(table, file_name);

    ASSERT_EQ(ret, 2);
    ASSERT_NE(table->fp, nullptr);
    ASSERT_STREQ(table->file_name, file_name);
    ASSERT_EQ(table->len, 2);
    for (idx = 0; idx < insert_count; idx++) {
        ASSERT_FALSE(table->cache_map[idx]);
    }
}

TEST(testTable, testLoadTableLarge) {
    char file_name[] = "./test/test.db";
    Table_t *table;
    int idx, ret;
    User_t sample_user;
    const int insert_count = 50;

    setup_sample_db_file(file_name, INIT_TABLE_SIZE+insert_count);

    table = new_Table(NULL);
    for (idx = 0; idx < insert_count; idx++) {
        setup_sample_user(&sample_user, idx);
        add_User(table, &sample_user);
    }

    ret = load_table(table, file_name);

    ASSERT_EQ(ret, INIT_TABLE_SIZE+insert_count);
    ASSERT_NE(table->fp, nullptr);
    ASSERT_STREQ(table->file_name, file_name);
    ASSERT_EQ(table->len, INIT_TABLE_SIZE+insert_count);
    ASSERT_TRUE(table->len <= table->capacity);
    for (idx = 0; idx < insert_count; idx++) {
        ASSERT_FALSE(table->cache_map[idx]);
    }
}

TEST(testTable, testLoadTableNULL) {
    Table_t *table;
    int idx, ret;
    User_t sample_user;
    const int insert_count = 50;

    table = new_Table(NULL);
    for (idx = 0; idx < insert_count; idx++) {
        setup_sample_user(&sample_user, idx);
        add_User(table, &sample_user);
    }

    ret = load_table(table, NULL);

    ASSERT_EQ(ret, insert_count);
    ASSERT_EQ(table->fp, nullptr);
    ASSERT_EQ(table->file_name, nullptr);
    ASSERT_EQ(table->len, insert_count);
    for (idx = 0; idx < insert_count; idx++) {
        ASSERT_TRUE(table->cache_map[idx]);
    }
}
