#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* arg[]) {

    int file_d = mkfifo("./temp", O_RDWR); // TODO check mkfifo ret val


    return 0;
}