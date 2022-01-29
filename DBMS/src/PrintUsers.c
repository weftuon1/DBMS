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
/// Print the user for given select fields format
///
void print_user(User_t *user, SelectArgs_t *sel_args)
{
    if(user == NULL)
		return;

	size_t idx;
    printf("(");

    for (idx = 0; idx < sel_args->fields_len; idx++) {
        if (!strncmp(sel_args->fields[idx], "*", 1)) {
            printf("%d, %s, %s, %d", user->id, user->name, user->email, user->age);
        } else {
            if (idx > 0) printf(", ");

            if (!strncmp(sel_args->fields[idx], "id", 2)) {
                printf("%d", user->id);
            } else if (!strncmp(sel_args->fields[idx], "name", 4)) {
                printf("%s", user->name);
            } else if (!strncmp(sel_args->fields[idx], "email", 5)) {
                printf("%s", user->email);
            } else if (!strncmp(sel_args->fields[idx], "age", 3)) {
                printf("%d", user->age);
            }
        }
    }
    printf(")\n");
}

///
/// Print the users for given aggregation functions
///
void print_aggregation(Table_t *table, int *idxList, size_t idxListLen, SelectArgs_t *sel_args)
{
    size_t idx;
    int limit = sel_args->limit;
    int offset = sel_args->offset;

    if (offset == -1) {
        offset = 0;
    }

	double *ans = (double *)malloc(sizeof(double) * (sel_args->fields_len/2 + 1));
	int ans_len = 0;
	
	if(idxList)
	{
		for(idx = 0; idx < sel_args->fields_len; idx += 2)
		{
			if((!strcmp(sel_args->fields[idx], "avg")) || (!strcmp(sel_args->fields[idx], "AVG")))
			{		
				if(!strcmp(sel_args->fields[idx+1], "id"))
				{
					double avg = 0;

					for(size_t cidx = 0; cidx < idxListLen; cidx++)
					{
						User_t *user = get_User(table, idxList[cidx]);
						if(user == NULL)
							continue;

						avg += (double)(user->id);
					}
					ans[idx/2] = (double)avg/idxListLen;
					ans_len++;
				}
				else if(!strcmp(sel_args->fields[idx+1], "age"))
				{
					double avg = 0;

					for(size_t cidx = 0; cidx < idxListLen; cidx++)
					{
						User_t *user = get_User(table, idxList[cidx]);
						if(user == NULL)
							continue;

						avg += (double)(user->age);
					}
					ans[idx/2] = (double)avg/idxListLen;
					ans_len++;
				}			
			}
			else if((!strcmp(sel_args->fields[idx], "sum")) || (!strcmp(sel_args->fields[idx], "SUM")))
			{		
				if(!strcmp(sel_args->fields[idx+1], "id"))
				{
					int sum = 0;

					for(size_t cidx = 0; cidx < idxListLen; cidx++)
					{
						User_t *user = get_User(table, idxList[cidx]);	
						if(user == NULL)
							continue;

						sum += user->id;
					}
					ans[idx/2] = (double)sum;
					ans_len++;
				}
				else if(!strcmp(sel_args->fields[idx+1], "age"))
				{			
					int sum = 0;

					for(size_t cidx = 0; cidx < idxListLen; cidx++)
					{
						User_t *user = get_User(table, idxList[cidx]);	
						if(user == NULL)
							continue;

						sum += user->age;
					}
					ans[idx/2] = (double)sum;
					ans_len++;
				}
			}
			else if((!strcmp(sel_args->fields[idx], "count")) || (!strcmp(sel_args->fields[idx], "COUNT")))
			{
				if(!strcmp(sel_args->fields[idx+1], "*"))
				{
					int count = 0;

					for(size_t cidx = 0; cidx < idxListLen; cidx++)
					{
						//User_t *user = get_User(table, idxList[cidx]);	
						//if(user == NULL)
						//	continue;
							
						count += 1;
					}
					ans[idx/2] = (double)count;
					ans_len++;
				}
				else if(!strcmp(sel_args->fields[idx+1], "id"))
				{
					int count = 0;

					for(size_t cidx = 0; cidx < idxListLen; cidx++)
					{
						//User_t *user = get_User(table, idxList[cidx]);	
						//if(user == NULL)
						//	continue;
							
						count += 1;
					}
					ans[idx/2] = (double)count;
					ans_len++;
				}
				else if(!strcmp(sel_args->fields[idx+1], "name"))
				{
					int count = 0;

					for(size_t cidx = 0; cidx < idxListLen; cidx++)
					{
						//User_t *user = get_User(table, idxList[cidx]);	
						//if(user == NULL)
						//	continue;
							
						count += 1;
					}
					ans[idx/2] = (double)count;
					ans_len++;
				}
				else if(!strcmp(sel_args->fields[idx+1], "email"))
				{
					int count = 0;

					for(size_t cidx = 0; cidx < idxListLen; cidx++)
					{
						//User_t *user = get_User(table, idxList[cidx]);	
						//if(user == NULL)
						//	continue;
							
						count += 1;
					}
					ans[idx/2] = (double)count;
					ans_len++;
				}
				else if(!strcmp(sel_args->fields[idx+1], "age"))
				{
					int count = 0;

					for(size_t cidx = 0; cidx < idxListLen; cidx++)
					{
						//User_t *user = get_User(table, idxList[cidx]);	
						//if(user == NULL)
						//	continue;
							
						count += 1;
					}
					ans[idx/2] = (double)count;
					ans_len++;
				}
			}
		}
	}
	else if(idxList == NULL)
	{
		for(idx = 0; idx < sel_args->fields_len; idx += 2)
		{
			if((!strcmp(sel_args->fields[idx], "avg")) || (!strcmp(sel_args->fields[idx], "AVG")))
			{
				if(!strcmp(sel_args->fields[idx+1], "id"))
				{
					double avg = 0;

					for(size_t cidx = 0; cidx < table->len; cidx++)
					{
						User_t *user = get_User(table, cidx);
						if(user == NULL)
							continue;
						
						avg += (double)(user->id);
					}
					ans[idx/2] = (double)avg/(table->len);
					ans_len++;
				}
				else if(!strcmp(sel_args->fields[idx+1], "age"))
				{
					double avg = 0;

					for(size_t cidx = 0; cidx < table->len; cidx++)
					{
						User_t *user = get_User(table, cidx);
						if(user == NULL)
							continue;
						
						avg += (double)(user->age);
					}
					ans[idx/2] = (double)avg/(table->len);
					ans_len++;
				}
			}
			else if((!strcmp(sel_args->fields[idx], "sum")) || (!strcmp(sel_args->fields[idx], "SUM")))
			{
				if(!strcmp(sel_args->fields[idx+1], "id"))
				{
					int sum = 0;

					for(size_t cidx = 0; cidx < table->len; cidx++)
					{
						User_t *user = get_User(table, cidx);
						if(user == NULL)
							continue;
						
						sum += user->id;
					}
					ans[idx/2] = (double)sum;
					ans_len++;
				}
				else if(!strcmp(sel_args->fields[idx+1], "age"))
				{
					int sum = 0;

					for(size_t cidx = 0; cidx < table->len; cidx++)
					{
						User_t *user = get_User(table, cidx);
						if(user == NULL)
							continue;
						
						sum += user->age;
					}
					ans[idx/2] = (double)sum;
					ans_len++;
				}
			}
			else if((!strcmp(sel_args->fields[idx], "count")) || (!strcmp(sel_args->fields[idx], "COUNT")))
			{
				if(!strcmp(sel_args->fields[idx+1], "*"))
				{
					int count = 0;

					for(size_t cidx = 0; cidx < table->len; cidx++)
					{
						User_t *user = get_User(table, cidx);	
						if(user == NULL)
							continue;

						count += 1;
					}
					ans[idx/2] = (double)count;
					ans_len++;
				}
				else if(!strcmp(sel_args->fields[idx+1], "id"))
				{
					int count = 0;

					for(size_t cidx = 0; cidx < table->len; cidx++)
					{
						User_t *user = get_User(table, cidx);	
						if(user == NULL)
							continue;

						count += 1;
					}
					ans[idx/2] = (double)count;
					ans_len++;
				}
				else if(!strcmp(sel_args->fields[idx+1], "name"))
				{
					int count = 0;

					for(size_t cidx = 0; cidx < table->len; cidx++)
					{
						User_t *user = get_User(table, cidx);	
						if(user == NULL)
							continue;

						count += 1;
					}
					ans[idx/2] = (double)count;
					ans_len++;
				}
				else if(!strcmp(sel_args->fields[idx+1], "email"))
				{
					int count = 0;

					for(size_t cidx = 0; cidx < table->len; cidx++)
					{
						User_t *user = get_User(table, cidx);	
						if(user == NULL)
							continue;

						count += 1;
					}
					ans[idx/2] = (double)count;
					ans_len++;
				}
				else if(!strcmp(sel_args->fields[idx+1], "age"))
				{
					int count = 0;

					for(size_t cidx = 0; cidx < table->len; cidx++)
					{
						User_t *user = get_User(table, cidx);	
						if(user == NULL)
							continue;

						count += 1;
					}
					ans[idx/2] = (double)count;
					ans_len++;
				}
			}
		}
	}

	if(ans_len > 0 && offset == 0 && (limit >=1 || limit == -1))	
	{
		printf("(");
		for(size_t cidx = 0; cidx < (unsigned)ans_len; cidx++)
		{
			if(cidx > 0)
				printf(", ");
					
			if((!strcmp(sel_args->fields[cidx*2], "avg")) || (!strcmp(sel_args->fields[cidx*2], "AVG")))
				printf("%.3lf", ans[cidx]);
			else if((!strcmp(sel_args->fields[cidx*2], "sum")) || (!strcmp(sel_args->fields[cidx*2], "SUM")))
				printf("%d", (int)ans[cidx]);
			else if((!strcmp(sel_args->fields[cidx*2], "count")) || (!strcmp(sel_args->fields[cidx*2], "COUNT")))
				printf("%d", (int)ans[cidx]);
		}
		printf(")\n");
	}

	free(ans);
}

///
/// Find the condition which is valid
///
int isValidCondition(User_t *user, SelectArgs_t *sel_args, size_t cidx)
{
	char* field;
	char* op;
	char* condition;
	
	field = sel_args->conditions[cidx];
	op = sel_args->conditions[cidx+1];
	condition = sel_args->conditions[cidx+2];

	if(!strncmp(field, "id", 2))
	{
		size_t data = user->id;
		size_t cond = atoi(condition);

		if(!strcmp(op, "!="))
		{
			if(data != cond)
				return 1;
		}
		else if(!strcmp(op, ">="))
		{
			if(data >= cond)
				return 1;
		}
		else if(!strcmp(op, "<="))
		{
			if(data <= cond)
				return 1;
		}
		else if(!strcmp(op, "="))
		{
			if(data == cond)
				return 1;
		}
		else if(!strcmp(op, ">"))
		{
			if(data > cond)
				return 1;
		}
		else if(!strcmp(op, "<"))
		{
			if(data < cond)
				return 1;
		}
	}
	else if(!strncmp(field, "name", 4))
	{
		char* data = user->name;

		if(!strcmp(op, "!="))
		{
			if(strcmp(data, condition))
				return 1;
		}
		else if(!strcmp(op, "="))
		{
			if(!strcmp(data, condition))
				return 1;
		}
	}
	else if(!strncmp(field, "email", 5))
	{
		char* data = user->email;

		if(!strcmp(op, "!="))
		{
			if(strcmp(data, condition))
				return 1;
		}
		else if(!strcmp(op, "="))
		{
			if(!strcmp(data, condition))
				return 1;
		}
	}
	else if(!strncmp(field, "age", 3))
	{
		size_t data = user->age;
		size_t cond = atoi(condition);

		if(!strcmp(op, "!="))
		{
			if(data != cond)
				return 1;
		}
		else if(!strcmp(op, ">="))
		{
			if(data >= cond)
				return 1;
		}
		else if(!strcmp(op, "<="))
		{
			if(data <= cond)
				return 1;
		}
		else if(!strcmp(op, "="))
		{
			if(data == cond)
				return 1;
		}
		else if(!strcmp(op, ">"))
		{
			if(data > cond)
				return 1;
		}
		else if(!strcmp(op, "<"))
		{
			if(data < cond)
				return 1;
		}
	}
	
	return 0;
}

///
/// Find the user who is valid for given where conditions format
///
int isValidWhere(User_t *user, SelectArgs_t *sel_args)
{
	if(user == NULL)
		return 0;

	size_t cidx;
	int condition = 0;
	char* logic = NULL;
	
	for(cidx = 0; cidx < sel_args->conditions_len; cidx += 4)
	{
		if(cidx >= 4)
			logic = sel_args->conditions[cidx-1];

		if(logic)
		{
			if((!strncmp(logic, "and", 3)) || (!strncmp(logic, "AND", 3)))
				condition = condition && isValidCondition(user, sel_args, cidx);
			else if((!strncmp(logic, "or", 2)) || (!strncmp(logic, "OR", 2)))
				condition = condition || isValidCondition(user, sel_args, cidx);
		}
		else
			condition = isValidCondition(user, sel_args, cidx);
	}

	return condition;
}

///
/// Print the users for given offset, limit restriction and where conditions
///
void print_users(Table_t *table, int *idxList, size_t idxListLen, Command_t *cmd) {
    size_t idx;
    int limit = cmd->cmd_args.sel_args.limit;
    int offset = cmd->cmd_args.sel_args.offset;

    if (offset == -1) {
        offset = 0;
    }
	/*
	if(cmd->cmd_args.sel_args.conditions)
	{
		size_t cidx;
		for(cidx = 0; cidx < cmd->cmd_args.sel_args.conditions_len; cidx++)
		{
			printf("%s ", cmd->cmd_args.sel_args.conditions[cidx]);
		}
		printf("\n");
	}
	*/
	/*	
	if(cmd->cmd_args.sel_args.fields)
	{
		size_t cidx;
		for(cidx = 0; cidx < cmd->cmd_args.sel_args.fields_len; cidx++)
		{
			printf("%s ", cmd->cmd_args.sel_args.fields[cidx]);
		}
		printf("\n");
	}
	*/
	if(cmd->cmd_args.sel_args.conditions)
	{
        size_t idxListCapacity;
		idxList = (int*)malloc(sizeof(int) * (INIT_TABLE_SIZE));
		idxListCapacity = (INIT_TABLE_SIZE);

		for (idx = 0; idx < table->len; idx++)
		{
            if(isValidWhere(get_User(table, idx), &(cmd->cmd_args.sel_args)))
			{
				if(idxListLen == idxListCapacity)
				{
					int *buf = (int*)malloc(sizeof(int) * (idxListCapacity*10));

					memcpy(buf, idxList, sizeof(int) * idxListLen);
					idxListCapacity *= 10;

					free(idxList);
					idxList = buf;
				}

				idxList[idxListLen] = idx;
				idxListLen++;
			}
        }
	}

    if(idxList)
	{
        if(cmd->cmd_args.sel_args.aggregation == 0)
		{
			for (idx = offset; idx < idxListLen; idx++) {
            	if (limit != -1 && (idx - offset) >= (unsigned)limit) {
                	break;
            	}
            	print_user(get_User(table, idxList[idx]), &(cmd->cmd_args.sel_args));
        	}
		}
        else if(cmd->cmd_args.sel_args.aggregation == 1)
		{
			print_aggregation(table, idxList, idxListLen, &(cmd->cmd_args.sel_args));
		}

		free(idxList);
    }
	else if((cmd->cmd_args.sel_args.conditions) == NULL)
	{
        if(cmd->cmd_args.sel_args.aggregation == 0)
		{
        	for (idx = offset; idx < table->len; idx++) {
            	if (limit != -1 && (idx - offset) >= (unsigned)limit) {
                	break;
            	}
            	print_user(get_User(table, idx), &(cmd->cmd_args.sel_args));
        	}
		}
        else if(cmd->cmd_args.sel_args.aggregation == 1)
		{
			print_aggregation(table, NULL, 0, &(cmd->cmd_args.sel_args));
		}
    }
}

///
/// Print the join users and likes for given offset, limit restriction and where conditions
///
void print_join(Table_t *table, Table2_t *table2, int *idxList, size_t idxListLen, Command_t *cmd)
{	
	//extern char array1[HASH_TABLE2_SIZE];
	//extern char array2[HASH_TABLE2_SIZE];
	size_t idx;
    int limit = cmd->cmd_args.sel_args.limit;
    int offset = cmd->cmd_args.sel_args.offset;

    if (offset == -1) {
        offset = 0;
    }

	if(cmd->cmd_args.sel_args.conditions)
	{
		size_t idxListCapacity;
		idxList = (int*)malloc(sizeof(int) * (INIT_TABLE_SIZE));
		idxListCapacity = (INIT_TABLE_SIZE);

		for(idx = 0; idx < table->len; idx++)
		{
			if(isValidWhere(get_User(table, idx), &(cmd->cmd_args.sel_args)))
			{
				if(idxListLen == idxListCapacity)
				{
					int *buf = (int*)malloc(sizeof(int) * (idxListCapacity*10));

					memcpy(buf, idxList, sizeof(int) * idxListLen);
					idxListCapacity *= 10;

					free(idxList);
					idxList = buf;
				}

				idxList[idxListLen] = idx;
				idxListLen++;
			}
   		}
	}

	if(offset == 0 && (limit >=1 || limit == -1))
	{
		if(idxList)
		{     
			if(!strcmp(cmd->cmd_args.sel_args.second_field, "id1"))
			{
				int count = 0;
				printf("(");
				for(idx = 0; idx < idxListLen; idx++)
				{			
					count += table2->hashTableId1[get_User(table, idxList[idx])->id];
				}
				printf("%d)\n", count);
			}
			else if(!strcmp(cmd->cmd_args.sel_args.second_field, "id2"))
			{
				int count = 0;
				printf("(");
				for(idx = 0; idx < idxListLen; idx++)
				{				
					count += table2->hashTableId2[get_User(table, idxList[idx])->id];
				}
				printf("%d)\n", count);
			}

			free(idxList);
		}
		else if((cmd->cmd_args.sel_args.conditions) == NULL)
		{
			if(!strcmp(cmd->cmd_args.sel_args.second_field, "id1"))
			{
				int count = 0;
				printf("(");
				for(idx = 0; idx < table->len; idx++)
				{			
					User_t *user = get_User(table, idx);	
					if(user == NULL)
						continue;

					count += table2->hashTableId1[(user->id)];
				}
				printf("%d)\n", count);
			}
			else if(!strcmp(cmd->cmd_args.sel_args.second_field, "id2"))
			{
				int count = 0;
				printf("(");
				for(idx = 0; idx < table->len; idx++)
				{
					User_t *user = get_User(table, idx);	
					if(user == NULL)
						continue;

					count += table2->hashTableId2[(user->id)];
				}
				printf("%d)\n", count);
			}
		}
	}
}

///
/// Update the user for given set fields format
///
void update_user(Table_t *table, User_t *user, UpdateArgs_t *up_args)
{
    if(user == NULL)
		return;
	
	size_t idx;

    for(idx = 0; idx < up_args->fields_len; idx += 2)
	{
        if(!strcmp(up_args->fields[idx], "id"))
		{
			//extern int hashTable[HASH_TABLE_SIZE];
			table->hashTable[(user->id) % (table->hashCapacity)] = 0;
			user->id = atoi(up_args->fields[idx+1]);
			table->hashTable[(user->id) % (table->hashCapacity)] = 1;
		}
        else if(!strcmp(up_args->fields[idx], "name"))
		{
			strcpy(user->name, up_args->fields[idx+1]);
		}
        else if(!strcmp(up_args->fields[idx], "email"))
		{
			strcpy(user->email, up_args->fields[idx+1]);
		}
        else if(!strcmp(up_args->fields[idx], "age"))
		{
			user->age = atoi(up_args->fields[idx+1]);
		}
    }
}

///
/// Find the update condition which is valid
///
int isValidUpdateCondition(User_t *user, UpdateArgs_t *up_args, size_t cidx)
{
	char* field;
	char* op;
	char* condition;
	
	field = up_args->conditions[cidx];
	op = up_args->conditions[cidx+1];
	condition = up_args->conditions[cidx+2];

	if(!strncmp(field, "id", 2))
	{
		size_t data = user->id;
		size_t cond = atoi(condition);

		if(!strcmp(op, "!="))
		{
			if(data != cond)
				return 1;
		}
		else if(!strcmp(op, ">="))
		{
			if(data >= cond)
				return 1;
		}
		else if(!strcmp(op, "<="))
		{
			if(data <= cond)
				return 1;
		}
		else if(!strcmp(op, "="))
		{
			if(data == cond)
				return 1;
		}
		else if(!strcmp(op, ">"))
		{
			if(data > cond)
				return 1;
		}
		else if(!strcmp(op, "<"))
		{
			if(data < cond)
				return 1;
		}
	}
	else if(!strncmp(field, "name", 4))
	{
		char* data = user->name;

		if(!strcmp(op, "!="))
		{
			if(strcmp(data, condition))
				return 1;
		}
		else if(!strcmp(op, "="))
		{
			if(!strcmp(data, condition))
				return 1;
		}
	}
	else if(!strncmp(field, "email", 5))
	{
		char* data = user->email;

		if(!strcmp(op, "!="))
		{
			if(strcmp(data, condition))
				return 1;
		}
		else if(!strcmp(op, "="))
		{
			if(!strcmp(data, condition))
				return 1;
		}
	}
	else if(!strncmp(field, "age", 3))
	{
		size_t data = user->age;
		size_t cond = atoi(condition);

		if(!strcmp(op, "!="))
		{
			if(data != cond)
				return 1;
		}
		else if(!strcmp(op, ">="))
		{
			if(data >= cond)
				return 1;
		}
		else if(!strcmp(op, "<="))
		{
			if(data <= cond)
				return 1;
		}
		else if(!strcmp(op, "="))
		{
			if(data == cond)
				return 1;
		}
		else if(!strcmp(op, ">"))
		{
			if(data > cond)
				return 1;
		}
		else if(!strcmp(op, "<"))
		{
			if(data < cond)
				return 1;
		}
	}
	
	return 0;
}

///
/// Find the user who is valid for given update where conditions format
///
int isValidUpdateWhere(User_t *user, UpdateArgs_t *up_args)
{
	if(user == NULL)
		return 0;
	
	size_t cidx;
	int condition = 0;
	char* logic = NULL;
	
	for(cidx = 0; cidx < up_args->conditions_len; cidx += 4)
	{
		if(cidx >= 4)
			logic = up_args->conditions[cidx-1];

		if(logic)
		{
			if((!strncmp(logic, "and", 3)) || (!strncmp(logic, "AND", 3)))
				condition = condition && isValidUpdateCondition(user, up_args, cidx);
			else if((!strncmp(logic, "or", 2)) || (!strncmp(logic, "OR", 2)))
				condition = condition || isValidUpdateCondition(user, up_args, cidx);
		}
		else
			condition = isValidUpdateCondition(user, up_args, cidx);
	}

	return condition;
}

///
/// Check field id whether existing in update where conditions
///
int isSetContainId(UpdateArgs_t *up_args)
{
	size_t idx;
	for(idx = 0; idx < up_args->fields_len; idx += 2)
	{
		if(!strcmp(up_args->fields[idx], "id"))
			return 1;
	}
	return 0;
}

///
/// Check new id whether existing in the table
///
int isTableContainNewId(Table_t *table, UpdateArgs_t *up_args)
{
	size_t idx;
	for(idx = 0; idx < up_args->fields_len; idx += 2)
	{
		if(!strcmp(up_args->fields[idx], "id"))
		{
			User_t *usr_ptr;
			for(size_t cidx = 0; cidx < table->len; cidx++)
			{
				usr_ptr = get_User(table, cidx);
				if(usr_ptr == NULL)
					continue;

				if((usr_ptr->id == (unsigned)atoi(up_args->fields[idx+1])))
					return 1;
			}
		}
	}
	return 0;
}

///
/// Update the users for given where conditions
///
void update_users(Table_t *table, int *idxList, size_t idxListLen, Command_t *cmd)
{ 
	if(isTableContainNewId(table, &(cmd->cmd_args.up_args)))
		return;
	
	size_t idx;
	/*
	if(cmd->cmd_args.up_args.conditions)
	{
		size_t cidx;
		for(cidx = 0; cidx < cmd->cmd_args.up_args.conditions_len; cidx++)
		{
			printf("%s ", cmd->cmd_args.up_args.conditions[cidx]);
		}
		printf("\n");
	}
	*/
	/*
	if(cmd->cmd_args.up_args.fields)
	{
		size_t cidx;
		for(cidx = 0; cidx < cmd->cmd_args.up_args.fields_len; cidx++)
		{
			printf("%s ", cmd->cmd_args.up_args.fields[cidx]);
		}
		printf("\n");
	}
	*/
	if(cmd->cmd_args.up_args.conditions)
	{
        size_t idxListCapacity;
		idxList = (int*)malloc(sizeof(int) * (INIT_TABLE_SIZE));
		idxListCapacity = (INIT_TABLE_SIZE);
		
		for (idx = 0; idx < table->len; idx++)
		{
            if(isValidUpdateWhere(get_User(table, idx), &(cmd->cmd_args.up_args)))
			{
				if(idxListLen == idxListCapacity)
				{
					int *buf = (int*)malloc(sizeof(int) * (idxListCapacity*10));

					memcpy(buf, idxList, sizeof(int) * idxListLen);
					idxListCapacity *= 10;

					free(idxList);
					idxList = buf;
				}

				idxList[idxListLen] = idx;
				idxListLen++;
			}
        }
	}

    if(idxList)
	{
		if((idxListLen >= 2) && (isSetContainId(&(cmd->cmd_args.up_args)) == 1))
			return;
		
		for(idx = 0; idx < idxListLen; idx++)
			update_user(table, get_User(table, idxList[idx]), &(cmd->cmd_args.up_args));
		
		free(idxList);
    }
	else if((cmd->cmd_args.up_args.conditions) == NULL)
	{
		if((table->len >= 2) && (isSetContainId(&(cmd->cmd_args.up_args)) == 1))
			return;

		for(idx = 0; idx < table->len; idx++)
			update_user(table, get_User(table, idx), &(cmd->cmd_args.up_args));
    }
}

///
/// Delete the user for given where conditions format
///
void delete_user(Table_t *table, User_t *user, size_t idx)
{	
	if(user == NULL)
		return;
	
	//extern int hashTable[HASH_TABLE_SIZE];
	table->hashTable[(user->id) % (table->hashCapacity)] = 0;

	table->cache_map[idx] = 2;
}

///
/// Find the delete condition which is valid
///
int isValidDeleteCondition(User_t *user, DeleteArgs_t *del_args, size_t cidx)
{
	char* field;
	char* op;
	char* condition;
	
	field = del_args->conditions[cidx];
	op = del_args->conditions[cidx+1];
	condition = del_args->conditions[cidx+2];

	if(!strncmp(field, "id", 2))
	{
		size_t data = user->id;
		size_t cond = atoi(condition);

		if(!strcmp(op, "!="))
		{
			if(data != cond)
				return 1;
		}
		else if(!strcmp(op, ">="))
		{
			if(data >= cond)
				return 1;
		}
		else if(!strcmp(op, "<="))
		{
			if(data <= cond)
				return 1;
		}
		else if(!strcmp(op, "="))
		{
			if(data == cond)
				return 1;
		}
		else if(!strcmp(op, ">"))
		{
			if(data > cond)
				return 1;
		}
		else if(!strcmp(op, "<"))
		{
			if(data < cond)
				return 1;
		}
	}
	else if(!strncmp(field, "name", 4))
	{
		char* data = user->name;

		if(!strcmp(op, "!="))
		{
			if(strcmp(data, condition))
				return 1;
		}
		else if(!strcmp(op, "="))
		{
			if(!strcmp(data, condition))
				return 1;
		}
	}
	else if(!strncmp(field, "email", 5))
	{
		char* data = user->email;

		if(!strcmp(op, "!="))
		{
			if(strcmp(data, condition))
				return 1;
		}
		else if(!strcmp(op, "="))
		{
			if(!strcmp(data, condition))
				return 1;
		}
	}
	else if(!strncmp(field, "age", 3))
	{
		size_t data = user->age;
		size_t cond = atoi(condition);

		if(!strcmp(op, "!="))
		{
			if(data != cond)
				return 1;
		}
		else if(!strcmp(op, ">="))
		{
			if(data >= cond)
				return 1;
		}
		else if(!strcmp(op, "<="))
		{
			if(data <= cond)
				return 1;
		}
		else if(!strcmp(op, "="))
		{
			if(data == cond)
				return 1;
		}
		else if(!strcmp(op, ">"))
		{
			if(data > cond)
				return 1;
		}
		else if(!strcmp(op, "<"))
		{
			if(data < cond)
				return 1;
		}
	}
	
	return 0;
}

///
/// Find the user who is valid for given delete where conditions format
///
int isValidDeleteWhere(User_t *user, DeleteArgs_t *del_args)
{
	if(user == NULL)
		return 0;

	size_t cidx;
	int condition = 0;
	char* logic = NULL;
	
	for(cidx = 0; cidx < del_args->conditions_len; cidx += 4)
	{
		if(cidx >= 4)
			logic = del_args->conditions[cidx-1];

		if(logic)
		{
			if((!strncmp(logic, "and", 3)) || (!strncmp(logic, "AND", 3)))
				condition = condition && isValidDeleteCondition(user, del_args, cidx);
			else if((!strncmp(logic, "or", 2)) || (!strncmp(logic, "OR", 2)))
				condition = condition || isValidDeleteCondition(user, del_args, cidx);
		}
		else
			condition = isValidDeleteCondition(user, del_args, cidx);
	}

	return condition;
}

///
/// Delete the users for given where conditions
///
void delete_users(Table_t *table, int *idxList, size_t idxListLen, Command_t *cmd)
{ 
	size_t idx;
	/*	
	if(cmd->cmd_args.del_args.conditions)
	{
		size_t cidx;
		for(cidx = 0; cidx < cmd->cmd_args.del_args.conditions_len; cidx++)
		{
			printf("%s ", cmd->cmd_args.del_args.conditions[cidx]);
		}
		printf("\n");
	}
	*/
	if(cmd->cmd_args.del_args.conditions)
	{
        size_t idxListCapacity;
		idxList = (int*)malloc(sizeof(int) * (INIT_TABLE_SIZE));
		idxListCapacity = (INIT_TABLE_SIZE);
		
		for (idx = 0; idx < table->len; idx++)
		{
            if(isValidDeleteWhere(get_User(table, idx), &(cmd->cmd_args.del_args)))
			{
				if(idxListLen == idxListCapacity)
				{
					int *buf = (int*)malloc(sizeof(int) * (idxListCapacity*10));

					memcpy(buf, idxList, sizeof(int) * idxListLen);
					idxListCapacity *= 10;

					free(idxList);
					idxList = buf;
				}
				
				idxList[idxListLen] = idx;
				idxListLen++;
			}
        }
	}

    if(idxList)
	{
		for(idx = 0; idx < idxListLen; idx++)
			delete_user(table, get_User(table, idxList[idx]), idxList[idx]);

		free(idxList);
    }
	else if((cmd->cmd_args.del_args.conditions) == NULL)
	{
		for(idx = 0; idx < table->len; idx++)
			delete_user(table, get_User(table, idx), idx);
    }
}
