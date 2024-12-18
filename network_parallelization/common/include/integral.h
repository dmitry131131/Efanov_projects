#pragma once

#include <stdlib.h>

typedef struct {
    double start;
    double end;
    int points;
    double result;
    unsigned int seed;
} ThreadTask;

typedef struct {
    double start;
    double end;

    size_t points;
} Task;

void monte_carlo_integration(Task* task, double* result);

void* thread_worker(void *arg); 
