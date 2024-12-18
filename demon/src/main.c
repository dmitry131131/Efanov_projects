#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syslog.h>
#include <signal.h>
#include <fcntl.h>
#include <dirent.h>
#include <syslog.h>
#include <pthread.h>

#include "utils.h"
#include "config.h"
#include "demon.h"

#define FIFO_PATH "/tmp/backup_daemon_fifo"

static volatile int keep_running = 1;
static pthread_mutex_t config_mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]) 
{
    int daemon_mode = 0;

    int opt;
    int pid_argument = 0;

    while ((opt = getopt(argc, argv, "dp:")) != -1) 
    {
        switch (opt) 
        {
        case 'd':
            daemonize();
            break;
        case 'p':
            pid_argument = atoi(optarg);

            if (pid_argument <= 0) 
            {
                fprintf(stderr, "Invalid PID: %s\n", optarg);
                exit(EXIT_FAILURE);
            }

            monitored_pid = pid_argument;
            printf("PID set to %d\n", monitored_pid);
            break;
        default:
            fprintf(stderr, "Usage: %s [-d] [-p pid]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    save_config();

    if (monitored_pid <= 0) 
    {
        fprintf(stderr, "Invalid monitored PID in configuration file.\n");
        exit(EXIT_FAILURE);
    }

    if (sampling_interval <= 0) 
    {
        fprintf(stderr, "Invalid sampling interval in configuration file.\n");
        exit(EXIT_FAILURE);
    }

    if (daemon_mode) daemonize();

    signal(SIGHUP, handle_signal);
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    // Open syslog
    openlog("backupd", LOG_PID | LOG_CONS, LOG_DAEMON);

    syslog(LOG_INFO, "Daemon started. Monitoring PID: %d, Interval: %dms", monitored_pid, sampling_interval);

    // Set up IPC channel
    setup_fifo(FIFO_PATH);

    // Start monitoring process
    monitor_process();

    syslog(LOG_INFO, "Daemon shutting down.");
    unlink(FIFO_PATH);

    // Close syslog
    closelog();
    return 0;
}
