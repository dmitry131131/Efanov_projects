#pragma once

#include "client.h"

#include <sys/select.h>

void* handle_request(void* arg);

void register_client(Clients* clients, const char* tx_path, const char* rx_path);

int handle_registration(Clients* clients, fd_set read_fds, int register_fd);

void run_server(Clients* clients, int register_fd);