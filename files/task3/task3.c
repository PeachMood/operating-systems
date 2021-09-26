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
    
    printf("%d %d\n", getuid(), geteuid());
    changeFile(file);

    uid_t realUid = getuid();
    uid_t effectiveUid = geteuid();
    if (realUid != effectiveUid) {
        setuid(realUid);
    }

    changeFile(file);
    printf("%d %d\n", getuid(), geteuid());

    return 0;
}