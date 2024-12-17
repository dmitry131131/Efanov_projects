#pragma once

#include <linux/limits.h>

#define MAX_CLIENTS 64

typedef struct _Client {
    int tx_fd;
    int rx_fd;
    
    char tx_path[PATH_MAX];
    char rx_path[PATH_MAX];

} Client;

typedef struct _Clients 
{
    Client* clients;

    int client_count;

} Clients;

Clients* clients_ctor(int client_count);

void clients_dtor(Clients* clients);