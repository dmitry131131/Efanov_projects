#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "config.h"
#include "monte_carlo.h"

void* monte_carlo(void* arg) {
    if (!arg) return NULL;

    Integral* integral = (Integral*) arg;   // Cast arg to integral type

    int points = POINTS_COUNT / THREADS_COUNT;
    int K = 0;

    double s = (integral->end - integral->start) * (integral->function(integral->end) - 0);
    double x = 0, y = 0;

    printf("Inter start: %lf\n", integral->start);
    printf("Inter end: %lf\n", integral->end);
    printf("Points: %d\n", points);

    srand((unsigned int) time(NULL));

    double max_val = integral->function(integral->end);
    double len     = (integral->end - integral->start);

    clock_t thread_time = clock();
    for (int i = 0; i < points; i++) {
        x = integral->start + (double) rand() / (double) RAND_MAX * len;
        y = (double) rand() / (double) RAND_MAX * max_val;

        if (fabs(y) - integral->function(x) < 0) K++;
    }
    thread_time = clock() - thread_time;

    printf("Time: %ld\n", thread_time);

    integral->result = (s / points) * K;

    return integral;
}