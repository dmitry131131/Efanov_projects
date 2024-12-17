#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "client.h"

Clients* clients_ctor(int client_count)
{
    Clients* clients = (Clients*) calloc (1, sizeof(Clients));

    if (clients == NULL) {
        perror("Unable to allocate memory for clients\n");

        return NULL;
    }

    clients->clients = (Client*) calloc (client_count, sizeof(Client));

    if (clients->clients == NULL) {
        perror("Unable to allocate memory for clients clients\n");

        free(clients);
        return NULL;
    }

    clients->client_count = 0;

    return clients;
}

void clients_dtor(Clients* clients)
{
    for (int i = 0; i < clients->client_count; ++i)
    {
        close(clients->clients[i].tx_fd);
        close(clients->clients[i].rx_fd);

        unlink(clients->clients[i].tx_path);
        unlink(clients->clients[i].rx_path);
    }

    free(clients->clients);

    free(clients);

    clients->client_count = 0;
}
