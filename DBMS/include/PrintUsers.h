#ifndef PRINT_USERS_H
#define PRINT_USERS_H
#include "Command.h"
#include "Table.h"

void print_user(User_t *user, SelectArgs_t *sel_args);
void print_aggregation(Table_t *table, int *idxList, size_t idxListLen, SelectArgs_t *sel_arg);
int isValidCondition(User_t *user, SelectArgs_t *sel_args, size_t cidx);
int isValidWhere(User_t *user, SelectArgs_t *sel_args);
void print_users(Table_t *table, int *idxList, size_t idxListLen, Command_t *cmd);

void print_join(Table_t *table, Table2_t *table2, int *idxList, size_t idxListLen, Command_t *cmd);

void update_user(Table_t *table, User_t *user, UpdateArgs_t *up_args);
int isValidUpdateCondition(User_t *user, UpdateArgs_t *up_args, size_t cidx);
int isValidUpdateWhere(User_t *user, UpdateArgs_t *up_args);
int isSetContainId(UpdateArgs_t *up_args);
int isTableContainNewId(Table_t *table, UpdateArgs_t *up_args);
void update_users(Table_t *table, int *idxList, size_t idxListLen, Command_t *cmd);

void delete_user(Table_t *table, User_t *user, size_t idx);
int isValidDeleteCondition(User_t *user, DeleteArgs_t *del_args, size_t cidx);
int isValidDeleteWhere(User_t *user, DeleteArgs_t *del_args);
void delete_users(Table_t *table, int *idxList, size_t idxListLen, Command_t *cmd);

#endif
