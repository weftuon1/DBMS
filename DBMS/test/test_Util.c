#include "gtest/gtest.h"
#include "Table.h"
#include "User.h"
#include "test_Util.h"

void setup_sample_user(User_t *user, int idx) {
    user->id = idx + 1;
    snprintf(user->name, MAX_USER_NAME+1, "user%d", idx+1);
    snprintf(user->email, MAX_USER_EMAIL+1, "user%d@example.com", idx+1);
    user->age = 20 + (idx % 7);
}

void setup_sample_db_file(char *file_name, int num_record) {
    User_t user;
    FILE *fp = fopen(file_name, "wb");
    int idx;
    struct stat st;
    for (idx=0; idx < num_record; idx++) {
        setup_sample_user(&user, idx);
        fwrite((void*)&user, sizeof(User_t), 1, fp);
    }
    fclose(fp);
    ASSERT_EQ(stat(file_name, &st), 0);
    ASSERT_EQ(st.st_size, sizeof(User_t)*num_record);
}

void check_sample_table_record_match(Table_t *table, int num_record) {
    User_t *tmp_user, sample_user;
    int idx;
    for (idx = 0; idx < num_record; idx++) {
        tmp_user = get_User(table, idx);
        ASSERT_NE(tmp_user, nullptr);

        setup_sample_user(&sample_user, idx);

        ASSERT_TRUE(table->cache_map[idx]);
        EXPECT_EQ(tmp_user->id, sample_user.id);
        EXPECT_STREQ(tmp_user->name, sample_user.name);
        EXPECT_STREQ(tmp_user->email, sample_user.email);
        EXPECT_EQ(tmp_user->age, sample_user.age);
    }
}
