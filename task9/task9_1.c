#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

void main(int argc, char *argv[]) {
    pid_t pid;
    if ((pid = fork()) > 0) {
        printf("The assignment said that the parent process should print something.");
    } else if (pid == 0) {
        execl("/bin/cat", "cat", "file.txt", (char *)0);
    } else {
        perror(argv[0]);
        exit(1);
    }
    exit(0);
}