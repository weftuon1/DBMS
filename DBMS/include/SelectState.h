#ifndef SELECT_STATE_H
#define SELECT_STATE_H

#include "Command.h"

void field_state_handler(Command_t *cmd, size_t arg_idx);
void table_state_handler(Command_t *cmd, size_t arg_idx);
void offset_state_handler(Command_t *cmd, size_t arg_idx);
void limit_state_handler(Command_t *cmd, size_t arg_idx);

void aggregation_state_handler(Command_t *cmd, size_t arg_idx);
void where_state_handler(Command_t *cmd, size_t arg_idx);

void join_state_handler(Command_t *cmd, size_t arg_idx);

#endif
