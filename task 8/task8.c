  #include <sys/types.h>
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include <errno.h>

  void main(int argc, char *argv[]) {
    if (argc < 2) {
      fprintf(stderr, "File was not specified.");
      exit(1);
    }

    char* file = argv[1];
    int fd = open(file, O_RDWR | O_CREAT);
    if (fd == -1) {
      fprintf(stderr, "Could not open the file %s", file);
      exit(2);
    }

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_start = SEEK_SET;
    lock.l_whence = 0;
    lock.l_len = 0;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
      fprintf(stderr, "Failed to capture file %s", file);
      exit(3);
    }
    char* command = malloc(strlen(file) + 10);
    command = strcat(command, "nano ");
    command = strcat(command, file);
    system(command);
    
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    exit(0);
  }