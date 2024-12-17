#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "common.h"
#include "server.h"
#include "client.h"

int main() {
    const char* register_fifo = REGISTER_FIFO_NAME;

    mkfifo(register_fifo, 0666);

    int register_fd = open(register_fifo, O_RDONLY);

    if (register_fd < 0) { 
        fprintf(stderr, "[ERROR] Unable to open FIFO: %s", register_fifo); 
        return -1; 
    }

    fprintf(stderr, "Server running...\n");

    Clients* clients = clients_ctor(MAX_CLIENTS);

    run_server(clients, register_fd);

    clients_dtor(clients);

    close(register_fd);

    unlink(register_fifo);

    return 0;
}
