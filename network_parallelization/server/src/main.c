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

int main() 
{
    int cores = sysconf(_SC_NPROCESSORS_ONLN); 
    if (cores <= 0) cores = 1;

    fprintf(stderr, "[Server] Server starting with %d cores\n", cores);

    pthread_t udp_thread, tcp_thread;

    if (pthread_create(&tcp_thread, NULL, tcp_server, NULL) != 0) 
    {
        fprintf(stderr, "[Server] Failed to create TCP thread\n");
    }

    if (pthread_create(&udp_thread, NULL, udp_discovery_response, (void*)&cores) != 0) 
    {
        fprintf(stderr, "[Server] Failed to create UDP thread\n");
    }


    pthread_join(udp_thread, NULL);
    pthread_join(tcp_thread, NULL);

    return 0;
}