#ifndef INPUT_BUF_H
#define INPUT_BUF_H

typedef struct {
    char *buffer;
    size_t buffer_len;
    ssize_t input_len;
} InputBuffer_t;

InputBuffer_t* new_InputBuffer();
void read_input(InputBuffer_t *input_buffer);
void clean_InputBuffer(InputBuffer_t *input_buffer);

#endif
