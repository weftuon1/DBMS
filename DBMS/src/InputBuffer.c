#include <stdio.h>
#include <stdlib.h>
#include "InputBuffer.h"

///
/// Allocate InputBuffer_t and initialize some attributes
/// Return: ptr of InputBuffer_t
///
InputBuffer_t* new_InputBuffer() {
    InputBuffer_t *input_buffer = (InputBuffer_t*) malloc(sizeof(InputBuffer_t));
    input_buffer->buffer = NULL;
    input_buffer->buffer_len = 0;
    input_buffer->input_len = 0;
    return input_buffer;
}

///
/// Read the input from stdin, then store into InputBuffer_t
/// TODO: accept different input source
///
void read_input(InputBuffer_t *input_buffer) {
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_len), stdin);
    ssize_t input_len;

    if (bytes_read <= 0) {
        printf("Error reading input\n");
        exit(1);
    }

    input_len = bytes_read;
    while (input_buffer->buffer[input_len - 1] == '\n'
            || input_buffer->buffer[input_len - 1] == '\r') {
        input_buffer->buffer[input_len - 1] = '\0';
        input_len--;
    }
    input_buffer->input_len = input_len;
}

///
/// Free the allocated buffer to store input string
///
void clean_InputBuffer(InputBuffer_t *input_buffer) {
    free(input_buffer->buffer);
    input_buffer->buffer = NULL;
    input_buffer->buffer_len = 0;
    input_buffer->input_len = 0;
}
