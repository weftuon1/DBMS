#include <string.h>
#include <stdlib.h>
#include "Command.h"
#include "DeleteState.h"
#include <stdio.h>

void where_delete_handler(Command_t *cmd, size_t arg_idx)
{
	if((!strcmp(cmd->args[arg_idx], "from")) && (!strcmp(cmd->args[arg_idx+1], "user")) && (arg_idx+2 == cmd->args_len))
	{	
		cmd->cmd_args.del_args.conditions = NULL;
		cmd->cmd_args.del_args.conditions_len = 0;
		return;
	}
	
	if(strcmp(cmd->args[arg_idx], "from") || strcmp(cmd->args[arg_idx+1], "user") || strcmp(cmd->args[arg_idx+2], "where"))
	{
		cmd->type = UNRECOG_CMD;
		return;
	}
	arg_idx += 3;

	cmd->cmd_args.del_args.conditions = NULL;
	cmd->cmd_args.del_args.conditions_len = 0;
	
	while(arg_idx < cmd->args_len)
	{
        if((!strncmp(cmd->args[arg_idx], "and", 3)) || (!strncmp(cmd->args[arg_idx], "AND", 3)))
		{
			add_delete_condition(cmd, cmd->args[arg_idx]);
		}
        else if((!strncmp(cmd->args[arg_idx], "or", 2)) || (!strncmp(cmd->args[arg_idx], "OR", 2)))
		{
			add_delete_condition(cmd, cmd->args[arg_idx]);
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
						add_delete_condition(cmd, cmd->args[arg_idx]);
					else if(strlen(source) > 2)
					{
						add_delete_condition(cmd, "!=");
						add_delete_condition(cmd, loc+2);
					}
				}
				else if(loc-source > 0)
				{
					char *token = (char*)malloc(sizeof(char) * strlen(source));
					strcpy(token, source);
					token = strtok(token, "!=");
					add_delete_condition(cmd, token);
					free(token);

					add_delete_condition(cmd, "!=");

					if((loc-source+2) < (unsigned)strlen(source))
						add_delete_condition(cmd, loc+2);
				}
			}
			else if((loc = strstr(source, ">=")))
			{
				if(loc-source == 0)
				{
					if(strlen(source) == 2)
						add_delete_condition(cmd, cmd->args[arg_idx]);
					else if(strlen(source) > 2)
					{
						add_delete_condition(cmd, ">=");
						add_delete_condition(cmd, loc+2);
					}
				}
				else if(loc-source > 0)
				{
					char *token = (char*)malloc(sizeof(char) * strlen(source));
					strcpy(token, source);
					token = strtok(token, ">=");
					add_delete_condition(cmd, token);
					free(token);
				
					add_delete_condition(cmd, ">=");

					if((loc-source+2) < (unsigned)strlen(source))
						add_delete_condition(cmd, loc+2);
				}
			}
			else if((loc = strstr(source, "<=")))
			{
				if(loc-source == 0)
				{
					if(strlen(source) == 2)
						add_delete_condition(cmd, cmd->args[arg_idx]);
					else if(strlen(source) > 2)
					{
						add_delete_condition(cmd, "<=");
						add_delete_condition(cmd, loc+2);
					}
				}
				else if(loc-source > 0)
				{
					char *token = (char*)malloc(sizeof(char) * strlen(source));
					strcpy(token, source);
					token = strtok(token, "<=");
					add_delete_condition(cmd, token);
					free(token);
				
					add_delete_condition(cmd, "<=");

					if((loc-source+2) < (unsigned)strlen(source))
						add_delete_condition(cmd, loc+2);
				}
			}
			else if((loc = strstr(source, "=")) && (strstr(source, "!=") == NULL) && (strstr(source, ">=") == NULL) && (strstr(source, "<=") == NULL))
			{
				if(loc-source == 0)
				{
					if(strlen(source) == 1)
						add_delete_condition(cmd, cmd->args[arg_idx]);
					else if(strlen(source) > 1)
					{
						add_delete_condition(cmd, "=");
						add_delete_condition(cmd, loc+1);
					}
				}
				else if(loc-source > 0)
				{
					char *token = (char*)malloc(sizeof(char) * strlen(source));
					strcpy(token, source);
					token = strtok(token, "=");
					add_delete_condition(cmd, token);
					free(token);
				
					add_delete_condition(cmd, "=");

					if((loc-source+1) < (unsigned)strlen(source))
						add_delete_condition(cmd, loc+1);
				}
			}
			else if((loc = strstr(source, ">")) && (strstr(source, ">=") == NULL))
			{
				if(loc-source == 0)
				{
					if(strlen(source) == 1)
						add_delete_condition(cmd, cmd->args[arg_idx]);
					else if(strlen(source) > 1)
					{
						add_delete_condition(cmd, ">");
						add_delete_condition(cmd, loc+1);
					}
				}
				else if(loc-source > 0)
				{
					char *token = (char*)malloc(sizeof(char) * strlen(source));
					strcpy(token, source);
					token = strtok(token, ">");
					add_delete_condition(cmd, token);
					free(token);
				
					add_delete_condition(cmd, ">");

					if((loc-source+1) < (unsigned)strlen(source))
						add_delete_condition(cmd, loc+1);
				}
			}
			else if((loc = strstr(source, "<")) && (strstr(source, "<=") == NULL))
			{
				if(loc-source == 0)
				{
					if(strlen(source) == 1)
						add_delete_condition(cmd, cmd->args[arg_idx]);
					else if(strlen(source) > 1)
					{
						add_delete_condition(cmd, "<");
						add_delete_condition(cmd, loc+1);
					}
				}
				else if(loc-source > 0)
				{
					char *token = (char*)malloc(sizeof(char) * strlen(source));
					strcpy(token, source);
					token = strtok(token, "<");
					add_delete_condition(cmd, token);
					free(token);
				
					add_delete_condition(cmd, "<");

					if((loc-source+1) < (unsigned)strlen(source))
						add_delete_condition(cmd, loc+1);
				}
			}
			else
			{
				add_delete_condition(cmd, source);
			}
        }
        arg_idx += 1;
    }

    return;
}
