#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "pipe.h"
#include "IO_methods.h"
#include "IO_settings.h"

size_t send_data_to_child(Pipe* self, File* file) {
    if (!self) return 0;
    if (!file) return 0;

    ssize_t read_size = 0;
    if((read_size = read(file->file_d, self->buffer.data, self->buffer.capacity))) {
        if (read_size < 0) return 1;
        self->buffer.size = (size_t) read_size;

        write(self->fd_direct[1], self->buffer.data, self->buffer.size);
    }

    return read_size;
}

size_t send_data_to_parent(Pipe* self, File* file) {
    if (!self) return 0;
    if (!file) return 0;

    ssize_t read_size = 0;
    while((read_size = read(file->file_d, self->buffer.data, self->buffer.capacity))) {
        if (read_size < 0) return 1;
        self->buffer.size = (size_t) read_size;

        write(self->fd_back[1], self->buffer.data, self->buffer.size);
        if (self->buffer.size < self->buffer.capacity) break;
    }

    return read_size;
}

size_t receive_data_from_parent(Pipe* self, File* receive_file) {
    if (!self) return 0;
    if (!receive_file) return 0;

    ssize_t read_size = 0;
    while((read_size = read(self->fd_direct[0], self->buffer.data, self->buffer.capacity))) {
        if (read_size < 0) return 1;
        self->buffer.size = (size_t) read_size;
        
        write(receive_file->file_d, self->buffer.data, self->buffer.size);
        if (self->buffer.size < self->buffer.capacity) break;
    }

    return 0;
}  

size_t receive_data_from_child(Pipe* self, File* receive_file) {
    if (!self) return 0;
    if (!receive_file) return 0;

    ssize_t read_size = 0;
    while((read_size = read(self->fd_back[0], self->buffer.data, self->buffer.capacity))) {
        if (read_size < 0) return 1;
        self->buffer.size = (size_t) read_size;
        
        write(receive_file->file_d, self->buffer.data, self->buffer.size);
        if (self->buffer.size < self->buffer.capacity) break;
    }

    return 0;
} 

File create_file(const char* filename) {
    File file = {};
    if (!filename) return file;

    file.file_d = open(filename, O_CREAT | O_RDWR | __O_CLOEXEC, S_IRWXU);
    assert(file.file_d != -1);

    struct stat file_stat = {};
    fstat(file.file_d, &file_stat);

    file.size = (size_t) file_stat.st_size;

    return file;
}

int delete_file(File* file) {
    if (!file) return 0;

    close(file->file_d);
    file->size = 0;

    return 0;
}