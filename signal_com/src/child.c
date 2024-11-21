#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <fcntl.h>

#include "config.h"
#include "child.h"

static void handler(int signo, siginfo_t *info, void *context) {
    extern sem_t* parent_sem;
    extern sem_t* child_sem;

    sem_wait(child_sem);
    printf("%c", info->si_value);
    sem_post(parent_sem);
}

int child_function() {
    extern sem_t* parent_sem;
    extern sem_t* child_sem;

    struct sigaction act = {};
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = handler;

    if (sigaction(1, &act, NULL)) {
        perror("Sigaction: ");
        return -1;
    }

    sem_post(parent_sem);

    while (1) {}

    return 0;
}