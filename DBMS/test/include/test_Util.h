#ifndef TEST_UTIL
#define TEST_UTIL

#include "Table.h"
#include "User.h"

void setup_sample_user(User_t *user, int idx);
void setup_sample_db_file(char *file_name, int num_record);
void check_sample_table_record_match(Table_t *table, int num_record);

#endif
