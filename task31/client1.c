#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/unistd.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/un.h>

#define PATH "socket"

void main() {
    char* filename = "socket";
    int fd;
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket(2) error");
        exit(1);
    }

    struct sockaddr_un addr; 
    memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, PATH);

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect(2) error");
        close(fd);
        exit(1);
    }

    char* string = "client1\n";
    int length = strlen(string);
    for (int i = 0; i < 100; ++i) {
        if(write(fd, string, length) < 0) {
            perror("write(1) error");
            close(fd);
            exit(1);
        }
    }

    close(fd);
    exit(0);
}