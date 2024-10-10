#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

#include "IO.h"
#include "msg_com.h"

const key_t MSG_ID = 150;

int main() {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork error ");
        return 1;
    }

    // Parent proccess (writer)
    if (pid) {
        int msg_d = msgget(MSG_ID, IPC_CREAT | 0666);
        if (msg_d == -1) {
            perror("parent msgget error ");
            return 1;
        }
        
        printf("MSG_D: %d\n", msg_d);

        msg sbuf = {.mtype = MSG_TYPE, .mtext = {}};

        int input_d = open_input_file(INPUT_FILENAME);
        if (input_d == -1) return 1;

        int msg_len = 0;
        while ((msg_len = read(input_d, sbuf.mtext, BUFFER_CAPACITY))) {
            if (msg_len == -1) {
                perror("read error ");
                return 1;
            }

            if((msgsnd(msg_d, (void *) &sbuf, msg_len, 0))) {
                perror("msgsnd error ");
                return 1;
            }
        }

        if((msgsnd(msg_d, (void *) &sbuf, 0, 0))) {
            perror("msgsnd error ");
            return 1;
        }

        close(input_d);

        int status = 0;
        waitpid(pid, &status, 0);
    }
    // Child process (receiver)
    else {
        int msg_d = msgget(MSG_ID, 0666);
        if (msg_d == -1) {
            perror("child msgget error ");
            return 1;
        }

        printf("MSG_D: %d\n", msg_d);

        msg rbuf = {};

        int output_d = crate_output_file(OUTPUT_FILENAME);
        if (output_d == -1) return 1;

        int message_len = 0;
        while ((message_len = msgrcv(msg_d, &rbuf, BUFFER_CAPACITY, MSG_TYPE, 0))) {
            if (message_len == -1) {
                perror("msgrcv error ");
                return 1;
            }

            if (write(output_d, rbuf.mtext, message_len) < 0) {
                perror("write error ");
                return 1;
            }
        }
        
        close(output_d);
        struct msqid_ds qstatus;
        msgctl(msg_d, IPC_RMID, &qstatus);
    }

    return 0;
}