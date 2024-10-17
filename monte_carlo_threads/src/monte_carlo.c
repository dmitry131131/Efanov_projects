#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "monte_carlo.h"

void* monte_carlo(void* arg) {
    if (!arg) return NULL;

    Integral* integral = (Integral*) arg;   // Cast arg to integral type

    int points = POINTS_COUNT;
    int K = 0;

    double s = (integral->end - integral->start) * (integral->function(integral->end) - 0);
    double x = 0, y = 0;

    srand((unsigned int) time(NULL));

    for (int i = 0; i < points; i++) {
        x = integral->start + (double) rand() / (double) RAND_MAX * (integral->end - integral->start);
        y = (double) rand() / (double) RAND_MAX * integral->function(integral->end);

        if (fabs(y) - integral->function(x) < 0) K++;
    }

    s = (s / points) * K;

    printf("Result: %lf\n", s);

    return NULL;
}