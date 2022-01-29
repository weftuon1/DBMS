#ifndef PRINT_LIKES_H
#define PRINT_LIKES_H
#include "Command.h"
#include "Table2.h"

void print_like(Like_t *like, SelectArgs_t *sel_args);
void print_aggregation2(Table2_t *table2, int *idxList, size_t idxListLen, SelectArgs_t *sel_arg);
int isValidCondition2(Like_t *like, SelectArgs_t *sel_args, size_t cidx);
int isValidWhere2(Like_t *like, SelectArgs_t *sel_args);
void print_likes(Table2_t *table2, int *idxList, size_t idxListLen, Command_t *cmd);

/*
void update_like(Table2_t *table2, Like_t *like, UpdateArgs_t *up_args);
int isValidUpdateCondition2(Like_t *like, UpdateArgs_t *up_args, size_t cidx);
int isValidUpdateWhere2(Like_t *like, UpdateArgs_t *up_args);
int isSetContainId2(UpdateArgs_t *up_args);
int isTable2ContainNewId(Table2_t *table2, UpdateArgs_t *up_args);
void update_likes(Table2_t *table2, int *idxList, size_t idxListLen, Command_t *cmd);

void delete_like(Table2_t *table2, Like_t *like, size_t idx);
int isValidDeleteCondition2(Like_t *like, DeleteArgs_t *del_args, size_t cidx);
int isValidDeleteWhere2(Like_t *like, DeleteArgs_t *del_args);
void delete_likes(Table2_t *table2, int *idxList, size_t idxListLen, Command_t *cmd);
*/
#endif
