#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <stdio.h>

void main() {
    pid_t pid;
    if ((pid = fork()) == 0) {
        execl("/bin/less", "less", "file.txt", (char*)0);
        exit(0);
    }

    wait(0);
    printf("The assignment said that the parent process should print something.");

    exit(0);
}