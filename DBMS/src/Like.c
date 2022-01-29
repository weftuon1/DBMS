#include <string.h>
#include <stdlib.h>
#include "Like.h"
#include "Command.h"

///
/// Allocate new space for Like_t
/// The caller should free the allocated space
///
Like_t* new_Like() {
    Like_t *new_like = (Like_t*)malloc(sizeof(Like_t));
    new_like->id1 = 0;
    new_like->id2 = 0;

    return new_like;
}

///
/// Transform from the input cmd to the Like_t
///
Like_t* command_to_Like(Command_t *cmd) {
    Like_t *like = new_Like();
    if (!like || !cmd) {
        return NULL;
    }
    if (cmd->args_len != 5) {
        return NULL;
    }
    like->id1 = atoi(cmd->args[3]);
    like->id2 = atoi(cmd->args[4]);

    return like;
}
