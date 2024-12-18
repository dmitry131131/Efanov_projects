#include <arpa/inet.h>
#include <bits/types/struct_timeval.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>

#include "client.h"
#include "buffer.h"
#include "integral.h"
#include "common.h"

Servers servers_create(int max_servers_count)
{
    Servers servers = {};

    servers.server = (ServerInfo*) calloc (max_servers_count, sizeof(ServerInfo));

    if (! servers.server) { fprintf(stderr, "Unable to allocate memory for servers"); }

    servers.count = 0;

    servers.total_cores = 0;

    return servers;
}

void servers_destroy(Servers* servers)
{
    free(servers->server);

    servers->count = 0;

    servers->total_cores = 0;
}

int discover_servers(Servers* servers, int max_servers)
{
    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (udp_socket < 0)
    {
        fprintf(stderr, "Unable to create UDP socket\n");

        return -1;
    }

    int broadcast = 1;

    if (setsockopt(udp_socket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
    {
        fprintf(stderr, "Failed to set socket option\n");
    }

    struct sockaddr_in broadcast_addr;
    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(UDP_PORT);
    broadcast_addr.sin_addr.s_addr = inet_addr(BROADCAST_IP);

    char message[] = "DISCOVER";
    if (sendto(udp_socket, message, strlen(message), 0, (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr)) < 0) 
    {
        fprintf(stderr, "Failed to send to broadcast\n");
    }

    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    Buffer* buffer = buffer_ctor(BUFFER_SIZE);

    int num_servers = 0;

    while (1) 
    {
        fd_set read_fds;
        struct timeval timeout;
        timeout.tv_sec = 5; // 1-second timeout
        timeout.tv_usec = 0;

        FD_ZERO(&read_fds);
        FD_SET(udp_socket, &read_fds);

        int activity = select(udp_socket + 1, &read_fds, NULL, NULL, &timeout);

        if (activity < 0)
        {
            fprintf(stderr, "Select failed\n");
        }

        if (activity == 0) break; // Timeout

        int bytes_received = recvfrom(udp_socket, buffer->buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&server_addr, &addr_len);

        fprintf(stderr, "Received\n");

        buffer->size = bytes_received;

        if (bytes_received < 0) 
        {
            fprintf(stderr, "Failed to recieve UDP packet\n");
        }

        buffer->buffer[bytes_received] = '\0';

        if (num_servers < max_servers) 
        {
            servers->server[num_servers].address = server_addr;

            servers->server[num_servers].cores = atoi(buffer->buffer); // server sends core count
            servers->total_cores += servers->server[num_servers].cores;

            fprintf(stderr, "[Client] Discovered server: %s with %d cores\n", inet_ntoa(server_addr.sin_addr), servers->server[num_servers].cores);

            num_servers++;
            servers->count++;
        }
    }

    fprintf(stderr, "[Client] Discovery complete. Found %d servers.\n", num_servers);

    close(udp_socket);
    buffer_dtor(buffer);

    return num_servers;
}

double communicate_task(ServerInfo* server, Task* task) 
{
    fprintf(stderr, "[Client] Connecting to server %s...\n", inet_ntoa(server->address.sin_addr));

    int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (tcp_socket < 0) {
        fprintf(stderr, "TCP connection failed\n");
    }

    int max_retries = 5;
    int retries = 0;

    while (connect(tcp_socket, (struct sockaddr *)&server->address, sizeof(server->address)) < 0) {
        if (++retries >= max_retries) {
            fprintf(stderr, "[Client] Failed to connect to server after retries\n");
            return 0;
        }
        fprintf(stderr, "[Client] Retrying connection to server...\n");

        sleep(1);
    }

    fprintf(stderr, "[Client] Sending task to server %s: [Start: %.2f, End: %.2f, Points: %zu]\n", 
        inet_ntoa(server->address.sin_addr), task->start, task->end, task->points);


    if (send(tcp_socket, task, sizeof(Task), 0) < 0) {
        fprintf(stderr, "[Client] Failed to send task\n");
    }

    double result = 0;

    if (recv(tcp_socket, &result, sizeof(double), 0) < 0) {
        fprintf(stderr, "[Client] Failed to receive result\n");
    }

    fprintf(stderr, "[Client] Received result from server %s: %.10f\n", 
           inet_ntoa(server->address.sin_addr), result);

    close(tcp_socket);

    return result;
}

double distribute_tasks(Servers* servers, Task* global_task) {
    fprintf(stderr, "[Client] setting the ports to TCP...\n");

    for (int i = 0; i < servers->count; ++i)
    {
        servers->server[i].address.sin_port = htons(TCP_PORT);
    }

    fprintf(stderr, "[Client] Give task to servers\n");

    double range = (global_task->end - global_task->start) / servers->total_cores;
    int points_per_core = global_task->points / servers->total_cores;

    double final_result = 0.0;

    int core_count = 0;

    for (int i = 0; i < servers->count; i++) 
    {
        int cur_server_cores = servers->server[i].cores;

        Task task = {
            .start  = global_task->start + (core_count * range),
            .end    = global_task->start + ((core_count + cur_server_cores) * range),
            .points = points_per_core * cur_server_cores,
        };

        fprintf(stderr, "[Client] Give task to server: %s\n"
            "start[%lg], end[%lg], points[%zu]\n", inet_ntoa(servers->server[i].address.sin_addr)
                                                 , task.start, task.end, task.points);

        final_result += communicate_task(&servers->server[i], &task);
    }

    return final_result;
}
