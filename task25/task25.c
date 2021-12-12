#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 27

void getString(char* string) {
    char symbol = 'a';
    int count = 0;
    while(symbol <= 'z') {
        if(count % 2 == 0) {
            string[count] = toupper(symbol);
        } else {
            string[count] = symbol;
        }
        count++;
        symbol++;
    }
    string[count] = '\0';
}

void toUpperCase(char* string) {
    int length = strlen(string);
    for(int i = 0; i < length; ++i) {
        if (string[i] > 'a') {
            string[i] = toupper(string[i]);
        }
    }
}

void main() {
    char buffer[BUFFER_SIZE];

    int fd[2]; 
    pid_t pid;
    if (pipe(fd) == -1) {
        perror("Could not create pipe.");
        exit(1);
    }

    if ((pid = fork()) > 0) {
        getString(buffer);
        printf("Input: %s\n", buffer);
        if(write(fd[1], buffer, BUFFER_SIZE) == -1) {
            perror("Parent process could not write to the pipe.");
        }
    } else if (pid == 0) {
        if(read(fd[0], buffer, BUFFER_SIZE) == -1) {
            perror("Child process read from the pipe.");
        } else {
            toUpperCase(buffer);
            printf("Ouput: %s\n", buffer);
        }
    } else {
        perror("Could not fork.");
        exit(2);
    }
    exit(0);
}