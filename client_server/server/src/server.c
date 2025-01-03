#include <errno.h>
#include <linux/limits.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "server.h"
#include "buffer.h"
#include "client.h"

void register_client(Clients* clients, const char* tx_path, const char* rx_path) {
    if (clients->client_count >= MAX_CLIENTS) { 
        fprintf(stderr, "[ERROR] Max client limit reached\n"); 
        
        return; 
    }

    if (mkfifo(tx_path, 0666) == -1 && errno == EEXIST) { 
        fprintf(stderr, "[ERROR] FIFO already exists: %s\n", tx_path); 
        
        return; 
    }
    if (mkfifo(rx_path, 0666) == -1 && errno == EEXIST) { 
        fprintf(stderr, "[ERROR] FIFO already exists: %s\n", rx_path); 

        return; 
    }

    fprintf(stderr, "[Server] Created FIFO's\n");

    int tx_fd = open(tx_path, O_RDWR);
    if (tx_fd == -1) { 
        fprintf(stderr, "[ERROR] Unable to open: %s\n", tx_path); 
        
        return;
    }
    
    int rx_fd = open(rx_path, O_RDWR);
    if (rx_fd == -1) { 
        fprintf(stderr, "[ERROR] Unable to open: %s\n", rx_path); 
        
        return; 
    }

    #define current_client clients->clients[clients->client_count]

    current_client.tx_fd = tx_fd;
    strcpy(current_client.tx_path, tx_path); 

    current_client.rx_fd = rx_fd;
    strcpy(current_client.rx_path, rx_path); 

    #undef current_client

    clients->client_count++; 

    write(rx_fd, "Registration successful\n", sizeof("Registration successful\n"));
    fprintf(stderr, "Client registered: T[%s], R[%s]\n", tx_path, rx_path);
}

void* handle_request(void* arg) {
    Client* client = (Client*) arg;

    fprintf(stderr, "[Tx]: %s, [Rx]: %s\n", client->tx_path, client->rx_path);

    int tx_fd = open(client->tx_path, O_RDWR);
    if (tx_fd == -1) {
        fprintf(stderr, "[ERROR] Unable to open tx FIFO: %s\n", client->tx_path);
        
        return NULL;
    }

    int rx_fd = open(client->rx_path, O_RDWR);
    if (rx_fd == -1) {
        fprintf(stderr, "[ERROR] Unable to open rx FIFO: %s\n", client->rx_path);
        
        close(tx_fd);
        return NULL;
    }

    Buffer* buffer = buffer_ctor(BUFFER_SIZE);
    if (!buffer) {
        return NULL;
    }

    if (read_from_file(buffer, client->tx_fd) == -1) return NULL;

    const char* filename = &buffer->buffer[sizeof("GET")];

    char* symb = strchr(buffer->buffer, '\n');

    if (symb) *symb = '\0';

    fprintf(stderr, "[%s] requested file: %s\n", client->tx_path, filename);

    int read_fd = open(filename, O_RDONLY);

    if (read_fd == -1) fprintf(stderr, "[ERROR] Unable to open: %s\n", filename);

    while (read_from_file(buffer, read_fd)) {
        write_to_file(buffer, client->rx_fd);
    }

    close(read_fd);

    buffer_dtor(buffer);

    fprintf(stderr, "[SERVER] Requested file granted -> [%s]\n", client->rx_path);

    return NULL;
}

int handle_registration(Clients* clients, fd_set read_fds, int register_fd) {
    if (FD_ISSET(register_fd, &read_fds)) {
        Buffer* buffer = buffer_ctor(2 * PATH_MAX);

        read_from_file(buffer, register_fd);

        if (buffer->size > 0) {
            fprintf(stderr, "Reading from FIFO registration info... \n");

            char tx_path[PATH_MAX];
            char rx_path[PATH_MAX];

            if (sscanf(buffer->buffer, "REGISTER %s %s", tx_path, rx_path) == 2) {
                fprintf(stderr, "Registratng client...\n");

                register_client(clients, tx_path, rx_path);
            }
            else if (! sscanf(buffer->buffer, "CLOSE")) {
                return 1;
            }
            else {
                fprintf(stderr, "[SERVER] Incorrect usage: REGISTER <fifo/tx/path> <fifo/rx/path>");
            }
        }
        else {
           // LOG("Nothing read from register fd\n");
        }

        buffer_dtor(buffer);
    }

    return 0;
}

void run_server(Clients* clients, int register_fd) {
    fd_set read_fds;

    int max_fd = register_fd;

    while (1) {
        FD_ZERO(&read_fds);

        FD_SET(register_fd, &read_fds);

        for (int i = 0; i < clients->client_count; ++i) {
            FD_SET(clients->clients[i].tx_fd, &read_fds);

            if (clients->clients[i].tx_fd > max_fd) {
                max_fd = clients->clients[i].tx_fd;
            }
        }

        int activity = select(max_fd + 1, &read_fds, NULL, NULL, 0); // should the last be 0 instead of NULL?
        
        if (activity < 0) fprintf(stderr, "Activity error\n");

        int result = handle_registration(clients, read_fds, register_fd);

        if (result) { 
            fprintf(stderr, "Closing server...\n"); 
            break; 
        }

        for (int i = 0; i < clients->client_count; i++) {
            if (FD_ISSET(clients->clients[i].tx_fd, &read_fds)) {
                pthread_t thread;
                pthread_create(&thread, NULL, handle_request, &clients->clients[i]);
                pthread_detach(thread);
            }
        }
    }
}
