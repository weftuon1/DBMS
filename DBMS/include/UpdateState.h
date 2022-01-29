#ifndef UPDATE_STATE_H
#define UPDATE_STATE_H

#include "Command.h"

void set_update_handler(Command_t *cmd, size_t arg_idx);
void where_update_handler(Command_t *cmd, size_t arg_idx);

#endif
