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

int main(const int argc, const char *argv[]) 
{
    if (argc < 5) 
    {
        fprintf(stderr, "Usage: %s <start> <end> <points> <servers>", argv[0]);
        return EXIT_FAILURE;
    }

    double start       = atof(argv[1]);
    double end         = atof(argv[2]);
    int    points      = atoi(argv[3]);
    int    max_servers = atoi(argv[4]);

    fprintf(stderr, "[Client] Integration range: [%.2f, %.2f], Points: %d, Max servers: %d\n", start, end, points, max_servers);

    Servers servers = servers_create(max_servers);

    int num_servers = discover_servers(&servers, max_servers);

    fprintf(stderr, "[Client] Total cores available: %d\n", servers.total_cores);

    if (num_servers == 0) 
    {
        fprintf(stderr, "[Client] No servers discovered\n");
        return EXIT_FAILURE;
    }

    fprintf(stderr, "[Client] Discovered %d servers\n", num_servers);

    Task global_task = {
        .start = start,
        .end   = end,
        .points = points,
    };

    double final_result = distribute_tasks(&servers, &global_task);

    fprintf(stderr, "[Client] Final result: %.10f\n", final_result);

    servers_destroy(&servers);

    return 0;
}