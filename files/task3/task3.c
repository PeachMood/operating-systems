#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

void changeFile(FILE* file) {
    if (((file = fopen("file.txt", "wb")) == NULL))
        perror("Could not open file.txt");
    else {
        fprintf(file, "File was changed!");
        fclose(file);
    }
}

int main() {
    FILE* file;
    changeFile(file);

    uid_t realUid = getuid();
    uid_t effectiveUid = geteuid();
    if (realUid != effectiveUid) {
        setuid(realUid);
    }

    printf("%d %d", getuid(), geteuid());
    changeFile(file);

    return 0;
}