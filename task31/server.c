#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <poll.h>

#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/unistd.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/un.h>

#define PATH "socket"
#define BACKLOG 10

void initPollfd(struct pollfd* fdarray) {
    memset(fdarray, 0, sizeof(fdarray));
    for (int i = 0; i < BACKLOG; ++i) {
        fdarray[i].fd = -1;
        fdarray[i].events = POLLIN;
    }
}

void addToPollfd(struct pollfd* fdarray, int fd) {
    for (int i = 0; i < BACKLOG; ++i) {
        if (fdarray[i].fd == -1) {
            fdarray[i].fd = fd;
            return;
        }
    }
}

void toUpperCase(char* string) {
    int length = strlen(string);
    for(int i = 0; i < length; ++i) {
        string[i] = toupper(string[i]);
    }
}

void main() {
    int listen_fd;
    
    if ((listen_fd = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0)) == -1) {
        perror("socket(2) error");
        exit(1);
    }
    
    struct sockaddr_un addr; 
    memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, PATH);
    unlink(PATH);
    if (bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind(2) error");
        close(listen_fd);
        exit(1);
    }

    if(listen(listen_fd, BACKLOG) == -1) {
        perror("listen(2) error");
        close(listen_fd);
        exit(1);
    }
    
    int timeout = 0;
    nfds_t nfds = BACKLOG;
    struct pollfd fdarray[BACKLOG];
    initPollfd(fdarray);

    for(;;) {
        int client_fd = accept(listen_fd, NULL, NULL);
        if (client_fd > -1) {
            addToPollfd(fdarray, client_fd);
        }


        poll(fdarray, nfds, timeout);
        char buffer[BUFSIZ];
		for (int i = 0; i < BACKLOG; ++i) {
		    if (fdarray[i].fd > 0 && (fdarray[i].revents & POLLIN) == POLLIN) {
                if(read(fdarray[i].fd, buffer, BUFSIZ) <= 0) {
					close(fdarray[i].fd);
					fdarray[i].fd = -1;
				} else {
					toUpperCase(buffer);
					puts(buffer);
				}
			}
		}
    }
    close(listen_fd);
    exit(0); 
}