#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


void main() {
    int fd = open("/dev/tty", O_RDONLY);

    static struct termios stored_settings;
    if (tcgetattr(fd, &stored_settings) == -1) {
        perror("Could not get current terminal attributes");
        exit(1);
    }



    struct termios new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON & ~ISIG);
	new_settings.c_cc[VMIN] = 1;
    tcsetattr(fd, TCSAFLUSH, &new_settings);

    char answer;

    printf("Please enter one character: ");
    fflush(stdout);
    read(fd, &answer, 1);
    printf("\n");
    switch (answer)
    {
    case 'a':
        printf("Congratulations, you entered the symbol a\n");
        break;
    case 'b':
        printf("Wow, it is the symbol b!\n");
        break;
    default:
        printf("Sorry, you have not entered a or b symbols.\n");
        break;
    }
    
	tcsetattr(fd, TCSAFLUSH, &stored_settings);
    
    // Extra task
    new_settings.c_lflag |= (ICANON | ISIG);
    printf("Stored settings: %u\n", stored_settings.c_lflag);
    printf("New settings: %u\n", new_settings.c_lflag);

    close(fd);
    exit(0);
}