#ifndef COMMAND_H
#define COMMAND_H

enum { 
    UNRECOG_CMD,
    BUILT_IN_CMD,
    QUERY_CMD,
};

enum {
    INSERT_CMD = 100,
    SELECT_CMD,
	UPDATE_CMD,
	DELETE_CMD,
};

typedef struct {
    char name[256];
    int len;
    unsigned char type;
} CMD_t;

extern CMD_t cmd_list[];

typedef struct SelectArgs {
    char **fields;
    size_t fields_len;
    int offset;
    int limit;
	char **conditions;
	size_t conditions_len;
	int aggregation;

    int join;
    char *first_table;
    char *second_table;
    char *first_field;
    char *second_field;
} SelectArgs_t;

typedef struct UpdateArgs {
	char **fields;
	size_t fields_len;
	char **conditions;
	size_t conditions_len;
} UpdateArgs_t;

typedef struct DeleteArgs {
	char **conditions;
	size_t conditions_len;
} DeleteArgs_t;

typedef union {
    SelectArgs_t sel_args;
	UpdateArgs_t up_args;
	DeleteArgs_t del_args;
} CmdArg_t;

typedef struct Command {
    unsigned char type;
    char **args;
    size_t args_len;
    size_t args_cap;
    CmdArg_t cmd_args;
} Command_t;

Command_t* new_Command();
int add_Arg(Command_t *cmd, const char *arg);
int add_select_field(Command_t *cmd, const char *argument);
int add_where_condition(Command_t *cmd, const char *argument);
int add_update_field(Command_t *cmd, const char *argument);
int add_update_condition(Command_t *cmd, const char *argument);
int add_delete_condition(Command_t *cmd, const char *argument);
void cleanup_Command(Command_t *cmd);

#endif
