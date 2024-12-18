#pragma once

#include <stddef.h>

#define BUFFER_SIZE  8192    // 8 KB 

typedef struct {
    char* buffer;
    size_t capacity;
    size_t size;
} Buffer;

Buffer* buffer_ctor (size_t capacity);

int buffer_dtor (Buffer* buffer);

int write_to_file (Buffer* buffer, int fileDesc);

int read_from_file (Buffer* buffer, int fileDesc);
