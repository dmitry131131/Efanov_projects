#pragma once

#include "integral.h"
#include <arpa/inet.h>

typedef struct {
    struct sockaddr_in address;
    int cores;
} ServerInfo;

typedef struct {
    ServerInfo* server;
    int count;
    int total_cores;
} Servers;

Servers servers_create(int max_server_count);

void servers_destroy(Servers* servers);
int discover_servers(Servers* servers, int max_servers);

double communicate_task(ServerInfo* server, Task* task); 

double distribute_tasks(Servers* servers, Task* global_task);