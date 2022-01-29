#include <string.h>
#include <stdlib.h>
#include "Command.h"
#include "UpdateState.h"
#include <stdio.h>

void set_update_handler(Command_t *cmd, size_t arg_idx)
{
	if(strcmp(cmd->args[arg_idx], "user") || strcmp(cmd->args[arg_idx+1], "set"))
	{
		cmd->type = UNRECOG_CMD;
		return;
	}
	arg_idx += 2;

	cmd->cmd_args.up_args.fields = NULL;
	cmd->cmd_args.up_args.fields_len = 0;
	cmd->cmd_args.up_args.conditions = NULL;
	cmd->cmd_args.up_args.conditions_len = 0;

	while(arg_idx < cmd->args_len)
	{
		char *source = cmd->args[arg_idx];
		char *loc;

		if(!strcmp(source, "where"))
		{
            where_update_handler(cmd, arg_idx+1);
            return;
        }
		/*
		else
		{
			if((loc = strstr(source, "=")))
			{
				if(loc - source > 0)
				{
					char *copy = (char*)malloc(sizeof(char) * strlen(source));
					strcpy(copy, source);
					
					char *token;
					token = strtok(copy, " =");
					while(token != NULL)
					{
						add_update_field(cmd, token);
						token = strtok(NULL, " =");
					}
					free(copy);
				}
			}
        }
		*/
		else if((loc = strstr(source, "=")))
		{
			if(loc-source == 0)
			{
				if(strlen(source) > 1)
				{
					add_update_field(cmd, loc+1);
				}
			}
			else if(loc-source > 0)
			{
				char *token = (char*)malloc(sizeof(char) * strlen(source));
				strcpy(token, source);
				token = strtok(token, "=");
				add_update_field(cmd, token);
				free(token);
			
				if((loc-source+1) < (unsigned)strlen(source))
					add_update_field(cmd, loc+1);
			}
		}
		else 
		{
			add_update_field(cmd, source);
		}
        arg_idx++;
    }
    return;
}

void where_update_handler(Command_t *cmd, size_t arg_idx)
{
    while(arg_idx < cmd->args_len)
	{
        if((!strncmp(cmd->args[arg_idx], "and", 3)) || (!strncmp(cmd->args[arg_idx], "AND", 3)))
		{
			add_update_condition(cmd, cmd->args[arg_idx]);
		}
        else if((!strncmp(cmd->args[arg_idx], "or", 2)) || (!strncmp(cmd->args[arg_idx], "OR", 2)))
		{
			add_update_condition(cmd, cmd->args[arg_idx]);
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
						add_update_condition(cmd, cmd->args[arg_idx]);
					else if(strlen(source) > 2)
					{
						add_update_condition(cmd, "!=");
						add_update_condition(cmd, loc+2);
					}
				}
				else if(loc-source > 0)
				{
					char *token = (char*)malloc(sizeof(char) * strlen(source));
					strcpy(token, source);
					token = strtok(token, "!=");
					add_update_condition(cmd, token);
					free(token);

					add_update_condition(cmd, "!=");

					if((loc-source+2) < (unsigned)strlen(source))
						add_update_condition(cmd, loc+2);
				}
			}
			else if((loc = strstr(source, ">=")))
			{
				if(loc-source == 0)
				{
					if(strlen(source) == 2)
						add_update_condition(cmd, cmd->args[arg_idx]);
					else if(strlen(source) > 2)
					{
						add_update_condition(cmd, ">=");
						add_update_condition(cmd, loc+2);
					}
				}
				else if(loc-source > 0)
				{
					char *token = (char*)malloc(sizeof(char) * strlen(source));
					strcpy(token, source);
					token = strtok(token, ">=");
					add_update_condition(cmd, token);
					free(token);
				
					add_update_condition(cmd, ">=");

					if((loc-source+2) < (unsigned)strlen(source))
						add_update_condition(cmd, loc+2);
				}
			}
			else if((loc = strstr(source, "<=")))
			{
				if(loc-source == 0)
				{
					if(strlen(source) == 2)
						add_update_condition(cmd, cmd->args[arg_idx]);
					else if(strlen(source) > 2)
					{
						add_update_condition(cmd, "<=");
						add_update_condition(cmd, loc+2);
					}
				}
				else if(loc-source > 0)
				{
					char *token = (char*)malloc(sizeof(char) * strlen(source));
					strcpy(token, source);
					token = strtok(token, "<=");
					add_update_condition(cmd, token);
					free(token);
				
					add_update_condition(cmd, "<=");

					if((loc-source+2) < (unsigned)strlen(source))
						add_update_condition(cmd, loc+2);
				}
			}
			else if((loc = strstr(source, "=")) && (strstr(source, "!=") == NULL) && (strstr(source, ">=") == NULL) && (strstr(source, "<=") == NULL))
			{
				if(loc-source == 0)
				{
					if(strlen(source) == 1)
						add_update_condition(cmd, cmd->args[arg_idx]);
					else if(strlen(source) > 1)
					{
						add_update_condition(cmd, "=");
						add_update_condition(cmd, loc+1);
					}
				}
				else if(loc-source > 0)
				{
					char *token = (char*)malloc(sizeof(char) * strlen(source));
					strcpy(token, source);
					token = strtok(token, "=");
					add_update_condition(cmd, token);
					free(token);
				
					add_update_condition(cmd, "=");

					if((loc-source+1) < (unsigned)strlen(source))
						add_update_condition(cmd, loc+1);
				}
			}
			else if((loc = strstr(source, ">")) && (strstr(source, ">=") == NULL))
			{
				if(loc-source == 0)
				{
					if(strlen(source) == 1)
						add_update_condition(cmd, cmd->args[arg_idx]);
					else if(strlen(source) > 1)
					{
						add_update_condition(cmd, ">");
						add_update_condition(cmd, loc+1);
					}
				}
				else if(loc-source > 0)
				{
					char *token = (char*)malloc(sizeof(char) * strlen(source));
					strcpy(token, source);
					token = strtok(token, ">");
					add_update_condition(cmd, token);
					free(token);
				
					add_update_condition(cmd, ">");

					if((loc-source+1) < (unsigned)strlen(source))
						add_update_condition(cmd, loc+1);
				}
			}
			else if((loc = strstr(source, "<")) && (strstr(source, "<=") == NULL))
			{
				if(loc-source == 0)
				{
					if(strlen(source) == 1)
						add_update_condition(cmd, cmd->args[arg_idx]);
					else if(strlen(source) > 1)
					{
						add_update_condition(cmd, "<");
						add_update_condition(cmd, loc+1);
					}
				}
				else if(loc-source > 0)
				{
					char *token = (char*)malloc(sizeof(char) * strlen(source));
					strcpy(token, source);
					token = strtok(token, "<");
					add_update_condition(cmd, token);
					free(token);
				
					add_update_condition(cmd, "<");

					if((loc-source+1) < (unsigned)strlen(source))
						add_update_condition(cmd, loc+1);
				}
			}
			else
			{
				add_update_condition(cmd, source);
			}
        }
        arg_idx += 1;
    }

    return;
}

