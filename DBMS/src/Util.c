#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include "Util.h"
#include "PrintUsers.h"
#include "PrintLikes.h"
#include "Command.h"
#include "Table.h"
#include "Table2.h"
#include "SelectState.h"
#include "UpdateState.h"
#include "DeleteState.h"

///
/// Allocate State_t and initialize some attributes
/// Return: ptr of new State_t
///
State_t* new_State() {
    State_t *state = (State_t*)malloc(sizeof(State_t));
    state->saved_stdout = -1;
    return state;
}

///
/// Print shell prompt
///
void print_prompt(State_t *state) {
    if (state->saved_stdout == -1) {
        printf("db > ");
    }
}

///
/// This function received an output argument
/// Return: category of the command
///
int parse_input(char *input, Command_t *cmd) {
    char *token;
    int idx;
    token = strtok(input, " ,\n");
	if(token == NULL)
		return UNRECOG_CMD;

    for (idx = 0; strlen(cmd_list[idx].name) != 0; idx++) {
        if (!strncmp(token, cmd_list[idx].name, cmd_list[idx].len)) {
            cmd->type = cmd_list[idx].type;
        }
    }
    while (token != NULL) {
        add_Arg(cmd, token);
        token = strtok(NULL, " ,\n");
    }
    return cmd->type;
}

///
/// Handle built-in commands
/// Return: command type
///
void handle_builtin_cmd(Table_t *table, Table2_t *table2, Command_t *cmd, State_t *state) {
    if (!strcmp(cmd->args[0], ".exit")) {
        archive_table(table);
		archive_table2(table2);
        exit(0);
    } else if (!strncmp(cmd->args[0], ".output", 7)) {
        if (cmd->args_len == 2) {
            if (!strncmp(cmd->args[1], "stdout", 6)) {
                close(1);
                dup2(state->saved_stdout, 1);
                state->saved_stdout = -1;
            } else if (state->saved_stdout == -1) {
                int fd = creat(cmd->args[1], 0644);
                state->saved_stdout = dup(1);
                if (dup2(fd, 1) == -1) {
                    state->saved_stdout = -1;
                }
                __fpurge(stdout); //This is used to clear the stdout buffer
            }
        }
    } else if (!strncmp(cmd->args[0], ".load", 5)) {
        if (cmd->args_len == 2) {
            load_table(table, cmd->args[1]);
        }
    } else if (!strncmp(cmd->args[0], ".help", 5)) {
        print_help_msg();
    }
}

///
/// Handle query type commands
/// Return: command type
///
int handle_query_cmd(Table_t *table, Table2_t *table2, Command_t *cmd) {
    if (!strcmp(cmd->args[0], "insert")) {
        handle_insert_cmd(table, table2, cmd);
        return INSERT_CMD;
    } else if (!strcmp(cmd->args[0], "select")) {
        handle_select_cmd(table, table2, cmd);
        return SELECT_CMD;
    } else if (!strcmp(cmd->args[0], "update")) {
        handle_update_cmd(table, cmd);
        return UPDATE_CMD;
    } else if (!strcmp(cmd->args[0], "delete")) {
        handle_delete_cmd(table, cmd);
        return DELETE_CMD;
    } else {
        return UNRECOG_CMD;
    }
}

///
/// The return value is the number of rows insert into table
/// If the insert operation success, then change the input arg
/// `cmd->type` to INSERT_CMD
///
int handle_insert_cmd(Table_t *table, Table2_t *table2, Command_t *cmd) {
    cmd->type = INSERT_CMD;
	int ret = 0;

	if(cmd->args_len == 7 && !strcmp(cmd->args[1], "into") && !strcmp(cmd->args[2], "user"))
	{
		User_t *user = command_to_User(cmd);
		if (user) {
			ret = add_User(table, user);
			if (ret > 0) {
				cmd->type = INSERT_CMD;
			}
		}
		return ret;
	}
	else if(cmd->args_len == 5 && !strcmp(cmd->args[1], "into") && !strcmp(cmd->args[2], "like"))
	{
		Like_t *like = command_to_Like(cmd);
		if (like) {
			ret = add_Like(table2, like);
			if (ret > 0) {
				cmd->type = INSERT_CMD;
			}
		}
		return ret;
	}
	return ret;
}

///
/// The return value is the number of rows select from table
/// If the select operation success, then change the input arg
/// `cmd->type` to SELECT_CMD
///
int handle_select_cmd(Table_t *table, Table2_t *table2, Command_t *cmd) {
    cmd->type = SELECT_CMD;
    field_state_handler(cmd, 1);

	if(cmd->args_len >= 4 && !strcmp(cmd->cmd_args.sel_args.first_table, "user"))
	{
		if(cmd->cmd_args.sel_args.join == 1)
			print_join(table, table2, NULL, 0,  cmd);
		else
			print_users(table, NULL, 0, cmd);

		return table->len;
	}
	else if(cmd->args_len >= 4 && !strcmp(cmd->cmd_args.sel_args.first_table, "like"))
	{
		print_likes(table2, NULL, 0, cmd);

		return table2->len;
	}

    return table->len;
}

///
/// The return value is the number of rows update from table
/// If the update operation success, then change the input arg
/// `cmd->type` to UPDATE_CMD
///
int handle_update_cmd(Table_t *table, Command_t *cmd) {
    cmd->type = UPDATE_CMD;
    set_update_handler(cmd, 1);

    update_users(table, NULL, 0, cmd);
    return table->len;
}

///
/// The return value is the number of rows delete from table
/// If the delete operation success, then change the input arg
/// `cmd->type` to DELETE_CMD
///
int handle_delete_cmd(Table_t *table, Command_t *cmd) {
    cmd->type = DELETE_CMD;
    where_delete_handler(cmd, 1);

    delete_users(table, NULL, 0, cmd);
    return table->len;
}

///
/// Show the help messages
///
void print_help_msg() {
    const char msg[] = "# Supported Commands\n"
    "\n"
    "## Built-in Commands\n"
    "\n"
    "  * .exit\n"
    "\tThis cmd archives the table, if the db file is specified, then exit.\n"
    "\n"
    "  * .output\n"
    "\tThis cmd change the output strategy, default is stdout.\n"
    "\n"
    "\tUsage:\n"
    "\t    .output (<file>|stdout)\n\n"
    "\tThe results will be redirected to <file> if specified, otherwise they will display to stdout.\n"
    "\n"
    "  * .load\n"
    "\tThis command loads records stored in <DB file>.\n"
    "\n"
    "\t*** Warning: This command will overwrite the records already stored in current table. ***\n"
    "\n"
    "\tUsage:\n"
    "\t    .load <DB file>\n\n"
    "\n"
    "  * .help\n"
    "\tThis cmd displays the help messages.\n"
    "\n"
    "## Query Commands\n"
    "\n"
    "  * insert\n"
    "\tThis cmd inserts one user record into table.\n"
    "\n"
    "\tUsage:\n"
    "\t    insert <id> <name> <email> <age>\n"
    "\n"
    "\t** Notice: The <name> & <email> are string without any whitespace character, and maximum length of them is 255. **\n"
    "\n"
    "  * select\n"
    "\tThis cmd will display all user records in the table.\n"
    "\n";
    printf("%s", msg);
}
