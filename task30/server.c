#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/unistd.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/un.h>

#define BUFFERSIZE 1024

void toUpperCase(char* string) {
    int length = strlen(string);
    for(int i = 0; i < length; ++i) {
        string[i] = toupper(string[i]);
    }
}

void main() {
    char* filename = "socket";

    int serverfd;
    if ((serverfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Не удалось создать сокет.");
        exit(1);
    }

    struct sockaddr_un addr; 
    memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, filename);
    if (bind(serverfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        fprintf(stderr, "Не удалось привязать сокет к адресу.");
        unlink(filename);
        exit(1);

    }

    if (listen(serverfd, 5) < 0) {
        fprintf(stderr, "Не прослушать запросы на соединение.");
        unlink(filename);
        exit(1);
    }    

    int clientfd;
    if ((clientfd = accept(serverfd, NULL, NULL)) < 0) {
        fprintf(stderr, "Не удалось принять запрос.");
        unlink(filename);
        exit(1);
    }

    int result;
    char buffer[BUFFERSIZE];
    if((result = read(clientfd, buffer, BUFFERSIZE)) < 0) {
        fprintf(stderr, "Не удалось считать данные с сокета.");
        unlink(filename);
        exit(1);
    }


    toUpperCase(buffer);
    puts(buffer);
    unlink(filename);
    close(clientfd);
    exit(0);
}