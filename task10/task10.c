#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <stdio.h>


void main(int argc, char *argv[]) {
    pid_t pid;
    if ((pid = fork() == 0)) {
        execvp(argv[1], &argv[1]);
    }

    int status;
    wait(&status);
    if (WIFEXITED(status)) {
        printf("child's exit status is: %d\n", WEXITSTATUS(status));
    }
    exit(0);
}
