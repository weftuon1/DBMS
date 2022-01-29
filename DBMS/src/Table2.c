#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "Table2.h"

//int hashTable2[HASH_TABLE2_SIZE] = {0};
//char array1[HASH_TABLE2_SIZE] = {0};
//char array2[HASH_TABLE2_SIZE] = {0};

///
/// Allocate a table2_t struct, then initialize some attributes, and
/// load table2 if the `file_name` is given
///
Table2_t *new_Table2(char *file_name) {
    Table2_t *table2 = (Table2_t*)malloc(sizeof(Table2_t));
    memset((void*)table2, 0, sizeof(Table2_t));
    table2->capacity = INIT_TABLE2_SIZE;
    table2->len = 0;
    table2->likes = (Like_t*)malloc(
                            sizeof(Like_t) * INIT_TABLE2_SIZE);
    table2->cache_map = (unsigned char*)malloc(sizeof(char)*INIT_TABLE2_SIZE);
    memset(table2->cache_map, 0, sizeof(char)*INIT_TABLE2_SIZE);
    table2->fp = NULL;
    table2->file_name = NULL;
    load_table2(table2, file_name);

    table2->hashTableId1 = (char*)malloc(sizeof(char) * HASH_TABLE2_SIZE);
    table2->hashTableId2 = (char*)malloc(sizeof(char) * HASH_TABLE2_SIZE);
    memset(table2->hashTableId1, 0, sizeof(char)*HASH_TABLE2_SIZE);
    memset(table2->hashTableId2, 0, sizeof(char)*HASH_TABLE2_SIZE);
    
    table2->hashCapacityId1 = HASH_TABLE2_SIZE;
    table2->hashCapacityId2 = HASH_TABLE2_SIZE;

    return table2;
}

///
/// Add the `Like_t` data to the given table2
/// If the table2 is full, it will allocate new space to store more
/// like data
/// return 1 when the data successfully add to table2
///
int add_Like(Table2_t *table2, Like_t *like) {
    size_t idx;
    //Like_t *like_ptr;
    if (!table2 || !like) {
        return 0;
    }
    // Check id doesn't exist in the table2
    /*
    for (idx = 0; idx < table2->len; idx++) {
        like_ptr = get_Like(table2, idx);
		if(like_ptr == NULL)
			continue;

        if (like_ptr->id1 == like->id1) {
            return 0;
        }
    }
    */
    size_t key = (like->id1);
    if(key >= table2->hashCapacityId1)
    {
        char *new_hash_buf = (char*)malloc(sizeof(char)*(key+1));
        memset(new_hash_buf, 0, sizeof(char)*(key+1));
        memcpy(new_hash_buf, table2->hashTableId1, sizeof(char)*(table2->hashCapacityId1));

        free(table2->hashTableId1);
        table2->hashTableId1 = new_hash_buf;
        table2->hashCapacityId1 = key+1;
    }
    key = (like->id2);
    if(key >= table2->hashCapacityId2)
    {
        char *new_hash_buf = (char*)malloc(sizeof(char)*(key+1));
        memset(new_hash_buf, 0, sizeof(char)*(key+1));
        memcpy(new_hash_buf, table2->hashTableId2, sizeof(char)*(table2->hashCapacityId2));

        free(table2->hashTableId2);
        table2->hashTableId2 = new_hash_buf;
        table2->hashCapacityId2 = key+1;
    }

    key = (like->id1);
    if(table2->hashTableId1[key] == 0)
        table2->hashTableId1[key] = 1;
    else
        return 0;

    key = (like->id2);
    table2->hashTableId2[key] += 1;

    if (table2->len == table2->capacity) {
        Like_t *new_like_buf = (Like_t*)malloc(sizeof(Like_t)*(table2->len+EXT_LEN));
        unsigned char *new_cache_buf = (unsigned char *)malloc(sizeof(unsigned char)*(table2->len+EXT_LEN));

        memcpy(new_like_buf, table2->likes, sizeof(Like_t)*table2->len);

        memset(new_cache_buf, 0, sizeof(unsigned char)*(table2->len+EXT_LEN));
        memcpy(new_cache_buf, table2->cache_map, sizeof(unsigned char)*table2->len);


        free(table2->likes);
        free(table2->cache_map);
        table2->likes = new_like_buf;
        table2->cache_map = new_cache_buf;
        table2->capacity += EXT_LEN;
    }
    idx = table2->len;
    memcpy((table2->likes)+idx, like, sizeof(Like_t));
    table2->cache_map[idx] = 1;
    table2->len++;

    //array1[(like->id1) % HASH_TABLE2_SIZE]++;
    //array2[(like->id2) % HASH_TABLE2_SIZE]++;

    return 1;
}

///
/// Return value is the archived table2 len
///
int archive_table2(Table2_t *table2) {
    size_t archived_len;
    struct stat st;

    if (table2->fp == NULL) {
        return 0;
    }
    if (stat(table2->file_name, &st) == 0) {
        archived_len = st.st_size / sizeof(Like_t);
    } else {
        archived_len = 0;
    }
    fwrite((void*)(table2->likes+archived_len), \
            sizeof(Like_t), table2->len-archived_len, \
            table2->fp);

    fclose(table2->fp);
    free(table2->file_name);
    table2->fp = NULL;
    table2->file_name = NULL;
    return table2->len;
}

///
/// Loading the db file will overwrite the existed records in table2,
/// only if the ``file_name`` is NULL
/// Return: the number of records in the db file
///
int load_table2(Table2_t *table2, char *file_name) {
    size_t archived_len;
    struct stat st;
    if (table2->fp != NULL) {
        fclose(table2->fp);
        free(table2->file_name);
        table2->fp = NULL;
        table2->file_name = NULL;
    }
    if (file_name != NULL) {
        table2->len = 0;
        memset(table2->cache_map, 0, sizeof(char)*INIT_TABLE2_SIZE);
        if (stat(file_name, &st) != 0) {
            //Create new file
            table2->fp = fopen(file_name, "wb");
        } else {
            archived_len = st.st_size / sizeof(Like_t);
            if (archived_len > table2->capacity) {
                Like_t *new_like_buf = (Like_t*)malloc(sizeof(Like_t)*(archived_len+EXT_LEN));
                unsigned char *new_cache_buf = (unsigned char *)malloc(sizeof(unsigned char)*(archived_len+EXT_LEN));

                memset(new_cache_buf, 0, sizeof(unsigned char)*(archived_len+EXT_LEN));

                free(table2->likes);
                free(table2->cache_map);
                table2->likes = new_like_buf;
                table2->cache_map = new_cache_buf;
                table2->capacity = archived_len+EXT_LEN;
            }
            table2->fp = fopen(file_name, "a+b");
            table2->len = archived_len;
        }
        table2->file_name = strdup(file_name);
    }
    return table2->len;
}

///
/// Return the like in table2 by the given index
///
Like_t* get_Like(Table2_t *table2, size_t idx) {
    size_t archived_len;
    struct stat st;
    if (!table2->cache_map[idx]) {
        if (idx > INIT_TABLE2_SIZE) {
            goto error;
        }
        if (stat(table2->file_name, &st) != 0) {
            goto error;
        }
        archived_len = st.st_size / sizeof(Like_t);
        if (idx >= archived_len) {
            //neither in file, nor in memory
            goto error;
        }

        fseek(table2->fp, idx*sizeof(Like_t), SEEK_SET);
        fread(table2->likes+idx, sizeof(Like_t), 1, table2->fp);
        table2->cache_map[idx] = 1;
    }

	if(table2->cache_map[idx] != 2)
    	return table2->likes+idx;
	else
		return NULL;

error:
    return NULL;
}

