#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

#include "buffer.h"

Buffer* buffer_ctor(size_t capacity) 
{
    Buffer* buffer = (Buffer*) calloc (1, sizeof(Buffer));
    if (buffer == NULL) {
        perror("Calloc error in allocate buffer struct");

        return NULL;
    }

    buffer->buffer = (char*) calloc (capacity, sizeof(char));

    if (buffer->buffer == NULL) {
        perror("Calloc error in allocate buffer");
        free(buffer->buffer);

        return NULL;
    }

    buffer->capacity = capacity;
    buffer->size = 0;

    return buffer;
}

int buffer_dtor(Buffer* buffer)
{
    assert(buffer);

    if (buffer->buffer == NULL) {
        perror("buffer is NULL");

        return 1;
    }

    free(buffer->buffer);

    buffer->size     = 0;
    buffer->capacity = 0;

    return 0;
}

int write_to_file(Buffer* buffer, int fileDesc)
{
    assert(buffer);

    int nBytes = write(fileDesc, buffer->buffer, buffer->size);

    if (nBytes == -1) {
        perror("Unable to write");

        return -1;
    }

    return nBytes;
}

int read_from_file(Buffer* buffer, int fileDesc)
{
    assert(buffer);

    buffer->size = read(fileDesc, buffer->buffer, buffer->capacity);
    
    if (buffer->size == -1) {
        perror("Read from file error");

        return -1;
    }

    return buffer->size;
}
