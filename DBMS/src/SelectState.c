#include <string.h>
#include <stdlib.h>
#include "Command.h"
#include "SelectState.h"
#include <stdio.h>

void field_state_handler(Command_t *cmd, size_t arg_idx) {
    cmd->cmd_args.sel_args.fields = NULL;
    cmd->cmd_args.sel_args.fields_len = 0;
    cmd->cmd_args.sel_args.limit = -1;
    cmd->cmd_args.sel_args.offset = -1;

    cmd->cmd_args.sel_args.conditions = NULL;
    cmd->cmd_args.sel_args.conditions_len = 0;
    cmd->cmd_args.sel_args.aggregation = 0;	

	cmd->cmd_args.sel_args.join = 0;
	cmd->cmd_args.sel_args.first_table = NULL;
	cmd->cmd_args.sel_args.second_table = NULL;
	cmd->cmd_args.sel_args.first_field = NULL;
	cmd->cmd_args.sel_args.second_field = NULL;

    while(arg_idx < cmd->args_len) {
        if (strstr(cmd->args[arg_idx], "(") && strstr(cmd->args[arg_idx], ")"))
		{
    		cmd->cmd_args.sel_args.aggregation = 1;
			aggregation_state_handler(cmd, arg_idx);
            return;
		}

        if (!strcmp(cmd->args[arg_idx], "*")) {
            add_select_field(cmd, cmd->args[arg_idx]);
        } else if (!strcmp(cmd->args[arg_idx], "id")) {
            add_select_field(cmd, cmd->args[arg_idx]);
        } else if (!strcmp(cmd->args[arg_idx], "name")) {
            add_select_field(cmd, cmd->args[arg_idx]);
        } else if (!strcmp(cmd->args[arg_idx], "email")) {
            add_select_field(cmd, cmd->args[arg_idx]);
        } else if (!strcmp(cmd->args[arg_idx], "age")) {
            add_select_field(cmd, cmd->args[arg_idx]);
		} else if (!strcmp(cmd->args[arg_idx], "id1")) {
            add_select_field(cmd, cmd->args[arg_idx]);
		} else if (!strcmp(cmd->args[arg_idx], "id2")) {
            add_select_field(cmd, cmd->args[arg_idx]);
        } else if (!strcmp(cmd->args[arg_idx], "from")) {
            table_state_handler(cmd, arg_idx+1);
            return;
        } else {
            cmd->type = UNRECOG_CMD;
            return;
        }
        arg_idx += 1;
    }

    cmd->type = UNRECOG_CMD;
    return;
}

void table_state_handler(Command_t *cmd, size_t arg_idx) {
    if (arg_idx < cmd->args_len
            && (!strcmp(cmd->args[arg_idx], "user") || !strcmp(cmd->args[arg_idx], "like")))
	{
        cmd->cmd_args.sel_args.first_table = (char*)malloc(sizeof(char) * (10));
		strcpy(cmd->cmd_args.sel_args.first_table, cmd->args[arg_idx]);
		arg_idx++;

        if (arg_idx == cmd->args_len) {
            return;
		} else if (!strcmp(cmd->args[arg_idx], "join")) {
            join_state_handler(cmd, arg_idx+1);
            return;
        } else if (!strcmp(cmd->args[arg_idx], "where")) {
            where_state_handler(cmd, arg_idx+1);
            return;
		} else if (!strcmp(cmd->args[arg_idx], "offset")) {
            offset_state_handler(cmd, arg_idx+1);
            return;
        } else if (!strcmp(cmd->args[arg_idx], "limit")) {
            limit_state_handler(cmd, arg_idx+1);
            return;
		}
    }

    cmd->type = UNRECOG_CMD;
    return;
}

void aggregation_state_handler(Command_t *cmd, size_t arg_idx)
{
	while(arg_idx < cmd->args_len)
	{
		if(!strcmp(cmd->args[arg_idx], "from"))
		{
            table_state_handler(cmd, arg_idx+1);
            return;
		}
		else
		{
			char *source = cmd->args[arg_idx];
			char *loc;

			if((loc = strstr(source, "(")) && strstr(source, ")"))
			{
				if(loc - source > 0)
				{
					char *copy = (char*)malloc(sizeof(char) * strlen(source));
					strcpy(copy, source);
					
					char *token;
					token = strtok(copy, " ()");
					while(token != NULL)
					{
						add_select_field(cmd, token);
						token = strtok(NULL, " ()");
					}
					free(copy);
				}
			}
		}
		arg_idx++;
	}

	return;
}

void join_state_handler(Command_t *cmd, size_t arg_idx)
{
	cmd->cmd_args.sel_args.join = 1;

	if(arg_idx+4 < cmd->args_len && !strcmp(cmd->args[arg_idx+1], "on") && !strcmp(cmd->args[arg_idx+3], "="))
	{
		cmd->cmd_args.sel_args.second_table = (char*)malloc(sizeof(char) * (10));
		strcpy(cmd->cmd_args.sel_args.second_table, cmd->args[arg_idx]);
		arg_idx += 2;

		cmd->cmd_args.sel_args.first_field = (char*)malloc(sizeof(char) * (10));
		strcpy(cmd->cmd_args.sel_args.first_field, cmd->args[arg_idx]);
		arg_idx += 2;

		cmd->cmd_args.sel_args.second_field = (char*)malloc(sizeof(char) * (10));
		strcpy(cmd->cmd_args.sel_args.second_field, cmd->args[arg_idx]);

		arg_idx++;
        if(arg_idx == cmd->args_len)
		{
            return;
        }
		else if (arg_idx < cmd->args_len && !strcmp(cmd->args[arg_idx], "where"))
		{
            where_state_handler(cmd, arg_idx+1);
            return;
        }
		else if (arg_idx < cmd->args_len && !strcmp(cmd->args[arg_idx], "offset"))
		{
            offset_state_handler(cmd, arg_idx+1);
            return;
        }
		else if (arg_idx < cmd->args_len && !strcmp(cmd->args[arg_idx], "limit"))
		{
            limit_state_handler(cmd, arg_idx+1);
            return;
        }
	}
	cmd->type = UNRECOG_CMD;
	return;
}

void where_state_handler(Command_t *cmd, size_t arg_idx)
{
    while(arg_idx < cmd->args_len)
	{
        if((!strcmp(cmd->args[arg_idx], "and")) || (!strcmp(cmd->args[arg_idx], "AND")))
		{
			add_where_condition(cmd, cmd->args[arg_idx]);
		}
        else if((!strcmp(cmd->args[arg_idx], "or")) || (!strcmp(cmd->args[arg_idx], "OR")))
		{
			add_where_condition(cmd, cmd->args[arg_idx]);
		}
		else if(!strcmp(cmd->args[arg_idx], "offset"))
		{
            offset_state_handler(cmd, arg_idx+1);
			return;
		}
		else if(!strcmp(cmd->args[arg_idx], "limit"))
		{
            limit_state_handler(cmd, arg_idx+1);
			return;
		}
		else
		{
			char *source = cmd->args[arg_idx];
			char *loc;

			if((loc = strstr(source, "!=")))
			{
				if(loc-source == 0)
				{
					if(strlen(source) == 2)
						add_where_condition(cmd, cmd->args[arg_idx]);
					else if(strlen(source) > 2)
					{
						add_where_condition(cmd, "!=");
						add_where_condition(cmd, loc+2);
					}
				}
				else if(loc-source > 0)
				{
					char *token = (char*)malloc(sizeof(char) * strlen(source));
					strcpy(token, source);
					token = strtok(token, "!=");
					add_where_condition(cmd, token);
					free(token);

					add_where_condition(cmd, "!=");

					if((loc-source+2) < (unsigned)strlen(source))
						add_where_condition(cmd, loc+2);
				}
			}
			else if((loc = strstr(source, ">=")))
			{
				if(loc-source == 0)
				{
					if(strlen(source) == 2)
						add_where_condition(cmd, cmd->args[arg_idx]);
					else if(strlen(source) > 2)
					{
						add_where_condition(cmd, ">=");
						add_where_condition(cmd, loc+2);
					}
				}
				else if(loc-source > 0)
				{
					char *token = (char*)malloc(sizeof(char) * strlen(source));
					strcpy(token, source);
					token = strtok(token, ">=");
					add_where_condition(cmd, token);
					free(token);
				
					add_where_condition(cmd, ">=");

					if((loc-source+2) < (unsigned)strlen(source))
						add_where_condition(cmd, loc+2);
				}
			}
			else if((loc = strstr(source, "<=")))
			{
				if(loc-source == 0)
				{
					if(strlen(source) == 2)
						add_where_condition(cmd, cmd->args[arg_idx]);
					else if(strlen(source) > 2)
					{
						add_where_condition(cmd, "<=");
						add_where_condition(cmd, loc+2);
					}
				}
				else if(loc-source > 0)
				{
					char *token = (char*)malloc(sizeof(char) * strlen(source));
					strcpy(token, source);
					token = strtok(token, "<=");
					add_where_condition(cmd, token);
					free(token);
				
					add_where_condition(cmd, "<=");

					if((loc-source+2) < (unsigned)strlen(source))
						add_where_condition(cmd, loc+2);
				}
			}
			else if((loc = strstr(source, "=")) && (strstr(source, "!=") == NULL) && (strstr(source, ">=") == NULL) && (strstr(source, "<=") == NULL))
			{
				if(loc-source == 0)
				{
					if(strlen(source) == 1)
						add_where_condition(cmd, cmd->args[arg_idx]);
					else if(strlen(source) > 1)
					{
						add_where_condition(cmd, "=");
						add_where_condition(cmd, loc+1);
					}
				}
				else if(loc-source > 0)
				{
					char *token = (char*)malloc(sizeof(char) * strlen(source));
					strcpy(token, source);
					token = strtok(token, "=");
					add_where_condition(cmd, token);
					free(token);
				
					add_where_condition(cmd, "=");

					if((loc-source+1) < (unsigned)strlen(source))
						add_where_condition(cmd, loc+1);
				}
			}
			else if((loc = strstr(source, ">")) && (strstr(source, ">=") == NULL))
			{
				if(loc-source == 0)
				{
					if(strlen(source) == 1)
						add_where_condition(cmd, cmd->args[arg_idx]);
					else if(strlen(source) > 1)
					{
						add_where_condition(cmd, ">");
						add_where_condition(cmd, loc+1);
					}
				}
				else if(loc-source > 0)
				{
					char *token = (char*)malloc(sizeof(char) * strlen(source));
					strcpy(token, source);
					token = strtok(token, ">");
					add_where_condition(cmd, token);
					free(token);
				
					add_where_condition(cmd, ">");

					if((loc-source+1) < (unsigned)strlen(source))
						add_where_condition(cmd, loc+1);
				}
			}
			else if((loc = strstr(source, "<")) && (strstr(source, "<=") == NULL))
			{
				if(loc-source == 0)
				{
					if(strlen(source) == 1)
						add_where_condition(cmd, cmd->args[arg_idx]);
					else if(strlen(source) > 1)
					{
						add_where_condition(cmd, "<");
						add_where_condition(cmd, loc+1);
					}
				}
				else if(loc-source > 0)
				{
					char *token = (char*)malloc(sizeof(char) * strlen(source));
					strcpy(token, source);
					token = strtok(token, "<");
					add_where_condition(cmd, token);
					free(token);
				
					add_where_condition(cmd, "<");

					if((loc-source+1) < (unsigned)strlen(source))
						add_where_condition(cmd, loc+1);
				}
			}
			else
			{
				add_where_condition(cmd, source);
			}
        }
        arg_idx += 1;
    }

    return;	
}

void offset_state_handler(Command_t *cmd, size_t arg_idx) {
    if (arg_idx < cmd->args_len) {
        cmd->cmd_args.sel_args.offset = atoi(cmd->args[arg_idx]);

        arg_idx++;

        if (arg_idx == cmd->args_len) {
            return;
        } else if (arg_idx < cmd->args_len
                && !strcmp(cmd->args[arg_idx], "limit")) {

            limit_state_handler(cmd, arg_idx+1);
            return;
        } else if (arg_idx < cmd->args_len
                && !strcmp(cmd->args[arg_idx], "where")) {

            where_state_handler(cmd, arg_idx+1);
            return;
		}
    }
    cmd->type = UNRECOG_CMD;
    return;
}

void limit_state_handler(Command_t *cmd, size_t arg_idx) {
    if (arg_idx < cmd->args_len) {
        cmd->cmd_args.sel_args.limit = atoi(cmd->args[arg_idx]);

        arg_idx++;

        if (arg_idx == cmd->args_len) {
            return;
        } else if (arg_idx < cmd->args_len
                && !strcmp(cmd->args[arg_idx], "offset")) {

            offset_state_handler(cmd, arg_idx+1);
            return;
        } else if (arg_idx < cmd->args_len
                && !strcmp(cmd->args[arg_idx], "where")) {

            where_state_handler(cmd, arg_idx+1);
            return;
        }
    }
    cmd->type = UNRECOG_CMD;
    return;
}
