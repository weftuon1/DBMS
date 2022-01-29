#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "InputBuffer.h"
#include "Util.h"
#include "Table.h"
#include "Table2.h"

int main(int argc, char **argv) {
    InputBuffer_t *input_buffer = new_InputBuffer();
    Command_t *cmd = new_Command();
    State_t *state = new_State();
    Table_t *table = NULL;
    Table2_t *table2 = NULL;
    int cmd_type;
    if (argc != 2) {
        table = new_Table(NULL);
        table2 = new_Table2(NULL);
    } else {
        table = new_Table(argv[1]);
        table2 = new_Table2(argv[1]);
    }
    if (table == NULL || table2 == NULL) {
        return 1;
    }
    for (;;) {
        print_prompt(state);
        read_input(input_buffer);
        cmd_type = parse_input(input_buffer->buffer, cmd);
        if (cmd_type == BUILT_IN_CMD) {
            handle_builtin_cmd(table, table2, cmd, state);
        } else if (cmd_type == QUERY_CMD) {
            handle_query_cmd(table, table2, cmd);
        } else if (cmd_type == UNRECOG_CMD) {
            printf("Unrecognized command '%s'.\n", input_buffer->buffer);
        }
        cleanup_Command(cmd);
        clean_InputBuffer(input_buffer);
    }
    return 0;
}
