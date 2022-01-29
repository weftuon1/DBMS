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
/// Print the like for given select fields format
///
void print_like(Like_t *like, SelectArgs_t *sel_args)
{
    if(like == NULL)
		return;

	size_t idx;
    printf("(");

    for (idx = 0; idx < sel_args->fields_len; idx++) {
        if (!strcmp(sel_args->fields[idx], "*"))
		{
            printf("%d, %d", like->id1, like->id2);
        }
		else
		{
            if (idx > 0)
				printf(", ");

            if (!strcmp(sel_args->fields[idx], "id1"))
                printf("%d", like->id1);
			else if (!strcmp(sel_args->fields[idx], "id2"))
                printf("%d", like->id2);
        }
    }
    printf(")\n");
}

///
/// Print the likes for given aggregation functions
///
void print_aggregation2(Table2_t *table2, int *idxList, size_t idxListLen, SelectArgs_t *sel_args)
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
				if(!strcmp(sel_args->fields[idx+1], "id1"))
				{
					double avg = 0;

					for(size_t cidx = 0; cidx < idxListLen; cidx++)
					{
						Like_t *like = get_Like(table2, idxList[cidx]);
						if(like == NULL)
							continue;

						avg += (double)(like->id1);
					}
					ans[idx/2] = (double)avg/idxListLen;
					ans_len++;
				}
				else if(!strcmp(sel_args->fields[idx+1], "id2"))
				{
					double avg = 0;

					for(size_t cidx = 0; cidx < idxListLen; cidx++)
					{
						Like_t *like = get_Like(table2, idxList[cidx]);
						if(like == NULL)
							continue;

						avg += (double)(like->id2);
					}
					ans[idx/2] = (double)avg/idxListLen;
					ans_len++;
				}			
			}
			else if((!strcmp(sel_args->fields[idx], "sum")) || (!strcmp(sel_args->fields[idx], "SUM")))
			{		
				if(!strcmp(sel_args->fields[idx+1], "id1"))
				{
					int sum = 0;

					for(size_t cidx = 0; cidx < idxListLen; cidx++)
					{
						Like_t *like = get_Like(table2, idxList[cidx]);	
						if(like == NULL)
							continue;

						sum += like->id1;
					}
					ans[idx/2] = (double)sum;
					ans_len++;
				}
				else if(!strcmp(sel_args->fields[idx+1], "id2"))
				{			
					int sum = 0;

					for(size_t cidx = 0; cidx < idxListLen; cidx++)
					{
						Like_t *like = get_Like(table2, idxList[cidx]);	
						if(like == NULL)
							continue;

						sum += like->id2;
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
						//Like_t *like = get_Like(table2, idxList[cidx]);	
						//if(like == NULL)
						//	continue;
							
						count += 1;
					}
					ans[idx/2] = (double)count;
					ans_len++;
				}
				else if(!strcmp(sel_args->fields[idx+1], "id1"))
				{
					int count = 0;

					for(size_t cidx = 0; cidx < idxListLen; cidx++)
					{
						//Like_t *like = get_Like(table2, idxList[cidx]);	
						//if(like == NULL)
						//	continue;
							
						count += 1;
					}
					ans[idx/2] = (double)count;
					ans_len++;
				}
				else if(!strcmp(sel_args->fields[idx+1], "id2"))
				{
					int count = 0;

					for(size_t cidx = 0; cidx < idxListLen; cidx++)
					{
						//Like_t *like = get_Like(table2, idxList[cidx]);	
						//if(like == NULL)
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
				if(!strcmp(sel_args->fields[idx+1], "id1"))
				{
					double avg = 0;

					for(size_t cidx = 0; cidx < table2->len; cidx++)
					{
						Like_t *like = get_Like(table2, cidx);
						if(like == NULL)
							continue;
						
						avg += (double)(like->id1);
					}
					ans[idx/2] = (double)avg/(table2->len);
					ans_len++;
				}
				else if(!strcmp(sel_args->fields[idx+1], "id2"))
				{
					double avg = 0;

					for(size_t cidx = 0; cidx < table2->len; cidx++)
					{
						Like_t *like = get_Like(table2, cidx);
						if(like == NULL)
							continue;
						
						avg += (double)(like->id2);
					}
					ans[idx/2] = (double)avg/(table2->len);
					ans_len++;
				}
			}
			else if((!strcmp(sel_args->fields[idx], "sum")) || (!strcmp(sel_args->fields[idx], "SUM")))
			{
				if(!strcmp(sel_args->fields[idx+1], "id1"))
				{
					int sum = 0;

					for(size_t cidx = 0; cidx < table2->len; cidx++)
					{
						Like_t *like = get_Like(table2, cidx);
						if(like == NULL)
							continue;
						
						sum += like->id1;
					}
					ans[idx/2] = (double)sum;
					ans_len++;
				}
				else if(!strcmp(sel_args->fields[idx+1], "id2"))
				{
					int sum = 0;

					for(size_t cidx = 0; cidx < table2->len; cidx++)
					{
						Like_t *like = get_Like(table2, cidx);
						if(like == NULL)
							continue;
						
						sum += like->id2;
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

					for(size_t cidx = 0; cidx < table2->len; cidx++)
					{
						Like_t *like = get_Like(table2, cidx);	
						if(like == NULL)
							continue;

						count += 1;
					}
					ans[idx/2] = (double)count;
					ans_len++;
				}
				else if(!strcmp(sel_args->fields[idx+1], "id1"))
				{
					int count = 0;

					for(size_t cidx = 0; cidx < table2->len; cidx++)
					{
						Like_t *like = get_Like(table2, cidx);	
						if(like == NULL)
							continue;

						count += 1;
					}
					ans[idx/2] = (double)count;
					ans_len++;
				}
				else if(!strcmp(sel_args->fields[idx+1], "id2"))
				{
					int count = 0;

					for(size_t cidx = 0; cidx < table2->len; cidx++)
					{
						Like_t *like = get_Like(table2, cidx);	
						if(like == NULL)
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
int isValidCondition2(Like_t *like, SelectArgs_t *sel_args, size_t cidx)
{
	char* field;
	char* op;
	char* condition;
	
	field = sel_args->conditions[cidx];
	op = sel_args->conditions[cidx+1];
	condition = sel_args->conditions[cidx+2];

	if(!strcmp(field, "id1"))
	{
		size_t data = like->id1;
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
	else if(!strcmp(field, "id2"))
	{
		size_t data = like->id2;
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
/// Find the like who is valid for given where conditions format
///
int isValidWhere2(Like_t *like, SelectArgs_t *sel_args)
{
	if(like == NULL)
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
			if((!strcmp(logic, "and")) || (!strcmp(logic, "AND")))
				condition = condition && isValidCondition2(like, sel_args, cidx);
			else if((!strcmp(logic, "or")) || (!strcmp(logic, "OR")))
				condition = condition || isValidCondition2(like, sel_args, cidx);
		}
		else
			condition = isValidCondition2(like, sel_args, cidx);
	}

	return condition;
}

///
/// Print the likes for given offset, limit restriction and where conditions
///
void print_likes(Table2_t *table2, int *idxList, size_t idxListLen, Command_t *cmd) {
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
		idxList = (int*)malloc(sizeof(int) * (INIT_TABLE2_SIZE));
		idxListCapacity = (INIT_TABLE2_SIZE);
		
		for (idx = 0; idx < table2->len; idx++) {
            if(isValidWhere2(get_Like(table2, idx), &(cmd->cmd_args.sel_args)))
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
            	print_like(get_Like(table2, idxList[idx]), &(cmd->cmd_args.sel_args));
        	}
		}
        else if(cmd->cmd_args.sel_args.aggregation == 1)
		{
			print_aggregation2(table2, idxList, idxListLen, &(cmd->cmd_args.sel_args));
		}

		free(idxList);
    }
	else if((cmd->cmd_args.sel_args.conditions) == NULL)
	{
        if(cmd->cmd_args.sel_args.aggregation == 0)
		{
        	for (idx = offset; idx < table2->len; idx++) {
            	if (limit != -1 && (idx - offset) >= (unsigned)limit) {
                	break;
            	}
            	print_like(get_Like(table2, idx), &(cmd->cmd_args.sel_args));
        	}
		}
        else if(cmd->cmd_args.sel_args.aggregation == 1)
		{
			print_aggregation2(table2, NULL, 0, &(cmd->cmd_args.sel_args));
		}
    }
}

/*
///
/// Update the like for given set fields format
///
void update_like(Table2_t *table2, Like_t *like, UpdateArgs_t *up_args)
{
    if(like == NULL)
		return;
	
	size_t idx;

    for(idx = 0; idx < up_args->fields_len; idx += 2)
	{
        if(!strcmp(up_args->fields[idx], "id1"))
		{
			//extern int hashTable2[HASH_TABLE2_SIZE];
			table2->hashTableId1[(like->id1) % (table2->hashCapacityId1)] = 0;
			like->id1 = atoi(up_args->fields[idx+1]);
			table2->hashTableId1[(like->id1) % (table2->hashCapacityId1)] = 1;
		}
        else if(!strcmp(up_args->fields[idx], "id2"))
		{
			like->id2 = atoi(up_args->fields[idx+1]);
		}
    }
}

///
/// Find the update condition which is valid
///
int isValidUpdateCondition2(Like_t *like, UpdateArgs_t *up_args, size_t cidx)
{
	char* field;
	char* op;
	char* condition;
	
	field = up_args->conditions[cidx];
	op = up_args->conditions[cidx+1];
	condition = up_args->conditions[cidx+2];

	if(!strcmp(field, "id1"))
	{
		size_t data = like->id1;
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
	else if(!strcmp(field, "id2"))
	{
		size_t data = like->id2;
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
/// Find the like who is valid for given update where conditions format
///
int isValidUpdateWhere2(Like_t *like, UpdateArgs_t *up_args)
{
	if(like == NULL)
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
			if((!strcmp(logic, "and")) || (!strcmp(logic, "AND")))
				condition = condition && isValidUpdateCondition2(like, up_args, cidx);
			else if((!strcmp(logic, "or")) || (!strcmp(logic, "OR")))
				condition = condition || isValidUpdateCondition2(like, up_args, cidx);
		}
		else
			condition = isValidUpdateCondition2(like, up_args, cidx);
	}

	return condition;
}

///
/// Check field id1 whether existing in update where conditions
///
int isSetContainId2(UpdateArgs_t *up_args)
{
	size_t idx;
	for(idx = 0; idx < up_args->fields_len; idx += 2)
	{
		if(!strcmp(up_args->fields[idx], "id1"))
			return 1;
	}
	return 0;
}

///
/// Check new id whether existing in the table2
///
int isTable2ContainNewId(Table2_t *table2, UpdateArgs_t *up_args)
{
	size_t idx;
	for(idx = 0; idx < up_args->fields_len; idx += 2)
	{
		if(!strcmp(up_args->fields[idx], "id1"))
		{
			Like_t *like_ptr;
			for(size_t cidx = 0; cidx < table2->len; cidx++)
			{
				like_ptr = get_Like(table2, cidx);
				if(like_ptr == NULL)
					continue;

				if((like_ptr->id1 == (unsigned)atoi(up_args->fields[idx+1])))
					return 1;
			}
		}
	}
	return 0;
}

///
/// Update the likes for given where conditions
///
void update_likes(Table2_t *table2, int *idxList, size_t idxListLen, Command_t *cmd)
{ 
	if(isTable2ContainNewId(table2, &(cmd->cmd_args.up_args)))
		return;
	
	size_t idx;
	
	if(cmd->cmd_args.up_args.conditions)
	{
		size_t idxListCapacity;
		idxList = (int*)malloc(sizeof(int) * (INIT_TABLE2_SIZE));
		idxListCapacity = (INIT_TABLE2_SIZE);
		
		for (idx = 0; idx < table2->len; idx++)
		{
            if(isValidUpdateWhere2(get_Like(table2, idx), &(cmd->cmd_args.up_args)))
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
		if((idxListLen >= 2) && (isSetContainId2(&(cmd->cmd_args.up_args)) == 1))
			return;
		
		for(idx = 0; idx < idxListLen; idx++)
			update_like(get_Like(table2, idxList[idx]), &(cmd->cmd_args.up_args));

		free(idxList);
    }
	else if((cmd->cmd_args.up_args.conditions) == NULL)
	{
		if((table2->len >= 2) && (isSetContainId2(&(cmd->cmd_args.up_args)) == 1))
			return;

		for(idx = 0; idx < table2->len; idx++)
			update_like(get_Like(table2, idx), &(cmd->cmd_args.up_args));
    }
}

///
/// Delete the like for given where conditions format
///
void delete_like(Table2_t *table2, Like_t *like, size_t idx)
{
	if(like == NULL)
		return;
		
	//extern int hashTable2[HASH_TABLE2_SIZE];
	table2->hashTableId1[(like->id1) % (table2->hashCapacityId1)] = 0;

	table2->cache_map[idx] = 2;
}

///
/// Find the delete condition which is valid
///
int isValidDeleteCondition2(Like_t *like, DeleteArgs_t *del_args, size_t cidx)
{
	char* field;
	char* op;
	char* condition;
	
	field = del_args->conditions[cidx];
	op = del_args->conditions[cidx+1];
	condition = del_args->conditions[cidx+2];

	if(!strcmp(field, "id1"))
	{
		size_t data = like->id1;
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
	else if(!strcmp(field, "id2"))
	{
		size_t data = like->id2;
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
/// Find the like who is valid for given delete where conditions format
///
int isValidDeleteWhere2(Like_t *like, DeleteArgs_t *del_args)
{
	if(like == NULL)
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
			if((!strcmp(logic, "and")) || (!strcmp(logic, "AND")))
				condition = condition && isValidDeleteCondition2(like, del_args, cidx);
			else if((!strcmp(logic, "or")) || (!strcmp(logic, "OR")))
				condition = condition || isValidDeleteCondition2(like, del_args, cidx);
		}
		else
			condition = isValidDeleteCondition2(like, del_args, cidx);
	}

	return condition;
}

///
/// Delete the likes for given where conditions
///
void delete_likes(Table2_t *table2, int *idxList, size_t idxListLen, Command_t *cmd)
{ 
	size_t idx;
	
	if(cmd->cmd_args.del_args.conditions)
	{
		size_t idxListCapacity;
		idxList = (int*)malloc(sizeof(int) * (INIT_TABLE2_SIZE));
		idxListCapacity = (INIT_TABLE2_SIZE);
		
		for (idx = 0; idx < table2->len; idx++)
		{
            if(isValidDeleteWhere2(get_Like(table2, idx), &(cmd->cmd_args.del_args)))
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
			delete_like(table2, get_Like(table2, idxList[idx]), idxList[idx]);

		free(idxList);
    }
	else if((cmd->cmd_args.del_args.conditions) == NULL)
	{
		for(idx = 0; idx < table2->len; idx++)
			delete_like(table2, get_Like(table2, idx), idx);
    }
}
*/
