#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define MAX_LINES_NUMBER 100
#define MAX_LINE_SIZE 512

int main() {
    int fileds;
    if ((fileds = open("file.txt", O_RDONLY)) == -1) {
        printf("Could not open file");
        return 0;
    };

    int number = 0;
    int size = 0;
    off_t linePosition[MAX_LINES_NUMBER];
    memset(linePosition, 0, sizeof(linePosition));
    int lineSize[MAX_LINES_NUMBER];
    memset(lineSize, 0, sizeof(lineSize));
    char symbol;
    while (read(fileds, &symbol, 1)) {
        if (symbol == '\n') {
            size++;
            lineSize[number++] = size;
            size = 0;
            linePosition[number] = lseek(fileds, 0, SEEK_CUR);
        } else {
            size++;
        }
    }
    number++;

    int line;
    char buffer[MAX_LINE_SIZE];
    while(1) {
        printf("Enter line number: ");
        scanf("%d", &line);
        if (!line) break;
        if (line > number) {
            printf("There are only %d lines in the file\n", number);
            continue;
        }
        line--;
        lseek(fileds, linePosition[line], SEEK_SET);
        read(fileds, buffer, lineSize[line]);
        printf("String: %s", buffer);
        printf("String size: %d\n", lineSize[line]);
        printf("\n");
        memset(buffer, 0, sizeof(buffer));
    }
    close(fileds);
    return 0;
}