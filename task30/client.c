#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/unistd.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/un.h>

#define BUFFERSIZE 1024

void main() {
    char* filename = "socket";
    int clientfd;
    if ((clientfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Не удалось создать сокет.");
        exit(1);
    }

    struct sockaddr_un addr; 
    memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, filename);

    if (connect(clientfd, (struct sockaddr*)&addr, sizeof(addr)) < -1) {
        perror("Не удалось присоединить сокет.");
        exit(1);
    }

    int result;
    char* message = "how much I want to pass the operating systems";
    if((result = write(clientfd, message, BUFFERSIZE)) < 0) {
        fprintf(stderr, "Не удалось отправить данные.");
        exit(1);
    }

    exit(0);
}