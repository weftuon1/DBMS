#ifndef TABLE_H
#define TABLE_H
#include <stdlib.h>
#include <stdio.h>
#include "User.h"

#define INIT_TABLE_SIZE 2000000
#define EXT_LEN 1000000
#define HASH_TABLE_SIZE 2000000

typedef struct Table {
    size_t capacity;
    size_t len;
    User_t *users;
    unsigned char *cache_map;
    FILE *fp;
    char *file_name;

    char *hashTable;
    size_t hashCapacity;
} Table_t;

Table_t *new_Table(char *file_name);
int add_User(Table_t *table, User_t *user);
int archive_table(Table_t *table);
int load_table(Table_t *table, char *file_name);
User_t* get_User(Table_t *table, size_t idx);

/*
typedef struct hashNode {
    User_t *val;
    struct hashNode *next;
} hashNode_t;

hashNode_t *hashTable[HASH_TABLE_SIZE];
hashNode_t *head;
*/

#endif
