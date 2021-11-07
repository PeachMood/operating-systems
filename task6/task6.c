#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define MAX_LINES_NUMBER 100
#define MAX_LINE_SIZE 512

int main() {
    int fileds1;
    if ((fileds1 = open("file.txt", O_RDONLY)) == -1) {
        printf("Could not open file");
        return 0;
    };

    int fileds2;
    if ((fileds2 = open("/dev/tty", O_RDONLY | O_NDELAY)) == -1) {
        printf("Could not open tty");
        return 0;
    }
    printf("%d\n", fileds2);

    int number = 0;
    int size = 0;
    off_t linePosition[MAX_LINES_NUMBER];
    memset(linePosition, 0, sizeof(linePosition));
    int lineSize[MAX_LINES_NUMBER];
    memset(lineSize, 0, sizeof(lineSize));
    char symbol;
    while (read(fileds1, &symbol, 1)) {
        if (symbol == '\n') {
            size++;
            lineSize[number++] = size;
            size = 0;
            linePosition[number] = lseek(fileds1, 0, SEEK_CUR);
        } else {
            size++;
        }
    }
    number++;

    int line;
    char buffer[MAX_LINE_SIZE];
    while(1) {
        printf("Enter line number: ");
        if (read(fileds2, buffer, MAX_LINE_SIZE) == 0) {
            for(line = 0; line < number; ++line) {
                lseek(fileds1, linePosition[line], SEEK_SET);
                read(fileds1, buffer, lineSize[line]);
                printf("String: %s", buffer);
                memset(buffer, 0, sizeof(buffer));
            }
            close(fileds1);
            close(fileds2);
            return 0;
        }

        if (!line) break;
        if (line > number) {
            printf("There are only %d lines in the file\n", number);
            continue;
        }
        line--;
        lseek(fileds1, linePosition[line], SEEK_SET);
        read(fileds1, buffer, lineSize[line]);
        printf("String: %s", buffer);
        memset(buffer, 0, sizeof(buffer));
    }
    close(fileds1);
    close(fileds2);
    return 0;
}