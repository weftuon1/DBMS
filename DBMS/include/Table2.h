#ifndef TABLE2_H
#define TABLE2_H
#include <stdlib.h>
#include <stdio.h>
#include "Like.h"

#define INIT_TABLE2_SIZE 2000000
#define EXT_LEN 1000000
#define HASH_TABLE2_SIZE 2000000

typedef struct Table2 {
    size_t capacity;
    size_t len;
    Like_t *likes;
    unsigned char *cache_map;
    FILE *fp;
    char *file_name;

    char *hashTableId1;
    char *hashTableId2;
    size_t hashCapacityId1;
    size_t hashCapacityId2;
} Table2_t;

Table2_t *new_Table2(char *file_name);
int add_Like(Table2_t *table2, Like_t *like);
int archive_table2(Table2_t *table2);
int load_table2(Table2_t *table2, char *file_name);
Like_t* get_Like(Table2_t *table2, size_t idx);

#endif
