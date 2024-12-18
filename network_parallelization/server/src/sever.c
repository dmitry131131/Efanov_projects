#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
#include <math.h>
#include <time.h>

#include "sever.h"
#include "integral.h"
#include "buffer.h"
#include "common.h"

void* tcp_connection_handler(void *arg) 
{
    int client_socket = *(int *)arg;

    Task task;
    double result = 0.0;

    if (recv(client_socket, &task, sizeof(Task), 0) < 0) 
    {
        fprintf(stderr, "[Server] Failed to receive task\n");
    }

    fprintf(stderr, "[Server] Recieved task: start=%.2f, end=%.2f, points=%zu\n", task.start, task.end, task.points);

    monte_carlo_integration(&task, &result);

    if (send(client_socket, &result, sizeof(double), 0) < 0) 
    {
        fprintf(stderr, "[Server] Failed to send result\n");
    }

    printf("[Server] Sent result: %.10f\n", result);

    close(client_socket);
    return NULL;
}

void* udp_discovery_response(void* arg) 
{
    int cores = *(int*) arg;
    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket < 0) fprintf(stderr, "UDP socket creation failed");

    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(BROADCAST_IP);
    server_addr.sin_port = htons(UDP_PORT);

    if (bind(udp_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {
        fprintf(stderr, "[Server] UDP bind failed");
    }

    fprintf(stderr, "[Server] UDP discovery server started. Listening on port %d\n", UDP_PORT);

    char core_message[BUFFER_SIZE];
    snprintf(core_message, sizeof(core_message), "%d", cores);

    while (1) 
    {
        socklen_t client_len = sizeof(client_addr);
        Buffer* buffer = buffer_ctor(BUFFER_SIZE);

        int bytes_received = recvfrom(udp_socket, buffer->buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);

        if (bytes_received < 0) 
        {
            fprintf(stderr, "[Server] Failed to receive UDP packet");
        }

        buffer->buffer[bytes_received] = '\0';

        if (strcmp(buffer->buffer, "DISCOVER") == 0) 
        {
            fprintf(stderr, "[Server] Received DISCOVER request from %s\n", inet_ntoa(client_addr.sin_addr));

            sendto(udp_socket, core_message, strlen(core_message), 0, (struct sockaddr *)&client_addr, client_len);
        }

        buffer_dtor(buffer);
    }

    close(udp_socket);
}

void* tcp_server(void* arg) 
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) 
    {
        fprintf(stderr, "[Server] TCP socket creation failed\n");
        return NULL;
    }

    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(TCP_PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {
        fprintf(stderr, "[Server] TCP bind failed\n");
        return NULL;
    }

    if (listen(server_socket, 1) < 0) 
    {
        fprintf(stderr, "[Server] TCP listen failed\n");
        return NULL;
    }

    fprintf(stderr, "[Server] TCP server started. Listening on port %d\n", TCP_PORT);

    while (1) 
    {
        socklen_t client_len = sizeof(client_addr);

        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);

        if (client_socket < 0) 
        {
            fprintf(stderr, "[Server] Failed to accept client connection: %s\n", strerror(errno));
        }

        fprintf(stderr, "[Server] Accepted connection from %s\n", inet_ntoa(client_addr.sin_addr));

        pthread_t thread_id;

        if (pthread_create(&thread_id, NULL, tcp_connection_handler, (void*)&client_socket) != 0) 
        {
            fprintf(stderr, "[Server] Failed to create thread: %s\n", strerror(errno));
            close(client_socket);
        }
        {
            pthread_detach(thread_id);
        }
    }

    close(server_socket);
}
