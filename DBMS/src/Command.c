#include <stdlib.h>
#include <string.h>
#include "Command.h"

CMD_t cmd_list[] = {
    { ".exit", 5, BUILT_IN_CMD },
    { ".output", 7, BUILT_IN_CMD },
    { ".load", 5, BUILT_IN_CMD },
    { ".help", 5, BUILT_IN_CMD },
    { "insert", 6, QUERY_CMD },
    { "select", 6, QUERY_CMD },
    { "update", 6, QUERY_CMD },
    { "delete", 6, QUERY_CMD },
    { "", 0, UNRECOG_CMD },
};

Command_t* new_Command() {
    Command_t *cmd = (Command_t*) malloc(sizeof(Command_t));
    memset(cmd, 0, sizeof(Command_t));

    cmd->type = UNRECOG_CMD;
    cmd->args = NULL;
    cmd->args_len = 0;
    cmd->args_cap = 0;
    return cmd;
}

///
/// Add argument to Command_t
/// Reallocate buffer if it is full.
///
int add_Arg(Command_t *cmd, const char *arg) {
    char **new_buf;
    if (cmd->args == NULL) {
        new_buf = (char **) malloc(sizeof(char*) * 5);
        if (new_buf == NULL)
            goto error;

        cmd->args = new_buf;
        cmd->args_cap = 5;
        memset((void*)cmd->args, 0, sizeof(char*) * 5);
    } else if (cmd->args_cap == cmd->args_len) {
        new_buf = (char **) malloc(
                                sizeof(char*) * (cmd->args_cap + 5));
        if (new_buf == NULL)
            goto error;

        memset((void*)new_buf, 0,
            sizeof(char*) * (cmd->args_cap+5));
        memcpy((void*)new_buf, (void *)cmd->args,
            sizeof(char*) * (cmd->args_cap));
        free(cmd->args);
        cmd->args = new_buf;
        cmd->args_cap += 5;
    }
    cmd->args[cmd->args_len] = strdup(arg);
    cmd->args_len++;
    return 0;

error:
    return 1;
}

///
/// Add select fields
///
int add_select_field(Command_t *cmd, const char *argument) {
    size_t fields_len = cmd->cmd_args.sel_args.fields_len;
    char **buf = (char**)malloc(sizeof(char*) * (fields_len+1));
    if (buf == NULL) {
        return 0;
    }

    if (cmd->cmd_args.sel_args.fields) {
        memcpy(buf, cmd->cmd_args.sel_args.fields, sizeof(char*) * fields_len);
        free(cmd->cmd_args.sel_args.fields);
    }

    cmd->cmd_args.sel_args.fields = buf;
    cmd->cmd_args.sel_args.fields[fields_len] = strdup(argument);
    cmd->cmd_args.sel_args.fields_len++;
    return 1;
}

///
/// Add select where conditions
///
int add_where_condition(Command_t *cmd, const char *argument) {
    size_t conditions_len = cmd->cmd_args.sel_args.conditions_len;
    char **buf = (char**)malloc(sizeof(char*) * (conditions_len+1));
    if (buf == NULL) {
        return 0;
    }

    if (cmd->cmd_args.sel_args.conditions) {
        memcpy(buf, cmd->cmd_args.sel_args.conditions, sizeof(char*) * conditions_len);
        free(cmd->cmd_args.sel_args.conditions);
    }

    cmd->cmd_args.sel_args.conditions = buf;
    cmd->cmd_args.sel_args.conditions[conditions_len] = strdup(argument);
    cmd->cmd_args.sel_args.conditions_len++;
    return 1;
}

///
/// Add update set fields
///
int add_update_field(Command_t *cmd, const char *argument) {
    size_t fields_len = cmd->cmd_args.up_args.fields_len;
    char **buf = (char**)malloc(sizeof(char*) * (fields_len+1));
    if (buf == NULL) {
        return 0;
    }

    if (cmd->cmd_args.up_args.fields) {
        memcpy(buf, cmd->cmd_args.up_args.fields, sizeof(char*) * fields_len);
        free(cmd->cmd_args.up_args.fields);
    }

    cmd->cmd_args.up_args.fields = buf;
    cmd->cmd_args.up_args.fields[fields_len] = strdup(argument);
    cmd->cmd_args.up_args.fields_len++;
    return 1;
}

///
/// Add update where conditions
///
int add_update_condition(Command_t *cmd, const char *argument) {
    size_t conditions_len = cmd->cmd_args.up_args.conditions_len;
    char **buf = (char**)malloc(sizeof(char*) * (conditions_len+1));
    if (buf == NULL) {
        return 0;
    }

    if (cmd->cmd_args.up_args.conditions) {
        memcpy(buf, cmd->cmd_args.up_args.conditions, sizeof(char*) * conditions_len);
        free(cmd->cmd_args.up_args.conditions);
    }

    cmd->cmd_args.up_args.conditions = buf;
    cmd->cmd_args.up_args.conditions[conditions_len] = strdup(argument);
    cmd->cmd_args.up_args.conditions_len++;
    return 1;
}

///
/// Add delete where conditions
///
int add_delete_condition(Command_t *cmd, const char *argument) {
    size_t conditions_len = cmd->cmd_args.del_args.conditions_len;
    char **buf = (char**)malloc(sizeof(char*) * (conditions_len+1));
    if (buf == NULL) {
        return 0;
    }

    if (cmd->cmd_args.del_args.conditions) {
        memcpy(buf, cmd->cmd_args.del_args.conditions, sizeof(char*) * conditions_len);
        free(cmd->cmd_args.del_args.conditions);
    }

    cmd->cmd_args.del_args.conditions = buf;
    cmd->cmd_args.del_args.conditions[conditions_len] = strdup(argument);
    cmd->cmd_args.del_args.conditions_len++;
    return 1;
}

///
/// Free the allocated arguments, but without free the argument buffer
///
/// Free the allocated arguments, but without free the argument buffer
///
void cleanup_Command(Command_t *cmd) {
    size_t idx;
    for (idx = 0; idx < cmd->args_cap; idx++) {
        free(cmd->args[idx]);
        cmd->args[idx] = NULL;
    }
    if (cmd->type == SELECT_CMD) {
        for (idx = 0; idx < cmd->cmd_args.sel_args.fields_len; idx++) {
            free(cmd->cmd_args.sel_args.fields[idx]);
            cmd->cmd_args.sel_args.fields[idx] = NULL;
        }
        free(cmd->cmd_args.sel_args.fields);
        cmd->cmd_args.sel_args.fields = NULL;
        cmd->cmd_args.sel_args.fields_len = 0;

        for (idx = 0; idx < cmd->cmd_args.sel_args.conditions_len; idx++) {
            free(cmd->cmd_args.sel_args.conditions[idx]);
            cmd->cmd_args.sel_args.conditions[idx] = NULL;
        }
        free(cmd->cmd_args.sel_args.conditions);
        cmd->cmd_args.sel_args.conditions = NULL;
        cmd->cmd_args.sel_args.conditions_len = 0;

        cmd->cmd_args.sel_args.aggregation = 0;

        cmd->cmd_args.sel_args.join = 0;
        free(cmd->cmd_args.sel_args.first_table);
        free(cmd->cmd_args.sel_args.second_table);
        free(cmd->cmd_args.sel_args.first_field);
        free(cmd->cmd_args.sel_args.second_field);
        cmd->cmd_args.sel_args.first_table = NULL;
        cmd->cmd_args.sel_args.second_table = NULL;
        cmd->cmd_args.sel_args.first_field = NULL;
        cmd->cmd_args.sel_args.second_field = NULL;
    }
    if (cmd->type == UPDATE_CMD) {
        for (idx = 0; idx < cmd->cmd_args.up_args.fields_len; idx++) {
            free(cmd->cmd_args.up_args.fields[idx]);
            cmd->cmd_args.up_args.fields[idx] = NULL;
        }
        free(cmd->cmd_args.up_args.fields);
        cmd->cmd_args.up_args.fields = NULL;
        cmd->cmd_args.up_args.fields_len = 0;

        for (idx = 0; idx < cmd->cmd_args.up_args.conditions_len; idx++) {
            free(cmd->cmd_args.up_args.conditions[idx]);
            cmd->cmd_args.up_args.conditions[idx] = NULL;
        }
        free(cmd->cmd_args.up_args.conditions);
        cmd->cmd_args.up_args.conditions = NULL;
        cmd->cmd_args.up_args.conditions_len = 0;
    }
    if (cmd->type == DELETE_CMD) {
        for (idx = 0; idx < cmd->cmd_args.del_args.conditions_len; idx++) {
            free(cmd->cmd_args.del_args.conditions[idx]);
            cmd->cmd_args.del_args.conditions[idx] = NULL;
        }
        free(cmd->cmd_args.del_args.conditions);
        cmd->cmd_args.del_args.conditions = NULL;
        cmd->cmd_args.del_args.conditions_len = 0;
    }
    cmd->type = UNRECOG_CMD;
    cmd->args_len = 0;
}
