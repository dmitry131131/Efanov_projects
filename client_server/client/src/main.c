#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "buffer.h"
#include "common.h"

int main(int argc, char* argv[])
{
    const char* register_fifo = REGISTER_FIFO_NAME;

    int register_fd = open(register_fifo, O_WRONLY);
    if (register_fd == -1) {
        perror("Can't open register FIFO");

        return -1;
    }

    Buffer* buffer = buffer_ctor(2 * PATH_MAX);
    if (!buffer) return -1;

    fgets(buffer->buffer, buffer->capacity, stdin);
    buffer->size = strlen(buffer->buffer);

    // printf("[buffer]: %s, size: %zu\n", buffer->buffer, buffer->size);

    if (write_to_file(buffer, register_fd) == -1) return -1;

    buffer_dtor(buffer);

    return 0;
}
