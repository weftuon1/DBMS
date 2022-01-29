#ifndef DB_UTIL_H
#define DB_UTIL_H
#include "Command.h"
#include "Table.h"
#include "Table2.h"

typedef struct State {
    int saved_stdout;
} State_t;

State_t* new_State();
void print_prompt(State_t *state);

int parse_input(char *input, Command_t *cmd);
void handle_builtin_cmd(Table_t *table, Table2_t *table2, Command_t *cmd, State_t *state);
int handle_query_cmd(Table_t *table, Table2_t *table2, Command_t *cmd);
int handle_insert_cmd(Table_t *table, Table2_t *table2, Command_t *cmd);
int handle_select_cmd(Table_t *table, Table2_t *table2, Command_t *cmd);
int handle_update_cmd(Table_t *table, Command_t *cmd);
int handle_delete_cmd(Table_t *table, Command_t *cmd);
void print_help_msg();

#endif
