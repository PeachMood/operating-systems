#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h> 

typedef struct Option {
    char type;
    char* argument;
} Option;

typedef struct Stack {
    Option* stack;
    int count;
    int size;
} Stack;

Stack options;
extern char **environ;

void initStack(int size) {
    if (size <= 0) {
        fprintf(stderr, "You can't create a stack of size %d.\n", size);
        return;
    } 
    options.count = 0;
    options.size = size;
    options.stack = malloc(size * sizeof(Option));
}

void push(Option newOption) {
    if (options.count == options.size) {
        options.size *= 2;
        options.stack = realloc(options.stack, options.size);
    }
    options.stack[options.count].type = newOption.type;
    options.stack[options.count].argument = newOption.argument;
    options.count++;
}

Option pop() {
    options.count--;
    return options.stack[options.count];
}

void deleteStack() {
    options.count = 0;
    options.size = 0;
    free(options.stack);
}

void executeOption(Option option) {
    char* end;
    char path[PATH_MAX];
    long newUlimit;
    long newSize;
    struct rlimit limit;

    switch (option.type)
    {
    case 'i':
        printf("Real and effective user IDs are %u and %u.\n", getuid(), geteuid());
        printf("Real and effective group IDs are %u and %u.\n", getgid(), getegid());
        break;
    case 's':
        if (setpgid(0, 0) == -1) 
           fprintf(stderr, "Failed to assign process as group leader.\n");
        break;
    case 'p':
        printf("Process ID of the calling process is %d.\n", getpid());
        printf("Process ID of the parent of the calling process is %d.\n", getppid());
        printf("Process group ID of the calling process is %d.\n", getpgid(0));
        break;
    case 'u':
        getrlimit(RLIMIT_FSIZE, &limit);
        printf("Soft limit on the size of a file is %lu.\n", limit.rlim_cur);
        printf("Hard limit on the size of a file is %lu.\n", limit.rlim_max);
        break;
    case 'U':
        getrlimit(RLIMIT_FSIZE, &limit);
        newUlimit = strtol(option.argument, &end, 10);
        if (errno == ERANGE || newUlimit <= 0)
            fprintf(stderr, "Incorrect new ulimit value %s.\n", option.argument);
        else if (newUlimit >= limit.rlim_max) 
            fprintf(stderr, "New ulimit size can't exceed the value of hard limit");
        else {
            limit.rlim_cur = newUlimit;
            setrlimit(RLIMIT_FSIZE, &limit);
        }
        break;
    case 'c':
        getrlimit(RLIMIT_CORE, &limit);
        printf("Maximum size of a core file that can be created is %lu.\n", limit.rlim_cur);
        break;
    case 'C':
        getrlimit(RLIMIT_CORE, &limit);
        newSize = strtol(option.argument, &end, 10);
        if (errno == ERANGE || newSize <= 0)
            fprintf(stderr, "Incorrect size %s.\n", option.argument);
        else if (newSize >= limit.rlim_max) 
            fprintf(stderr, "New size can't exceed the value of hard limit");
        else {
            limit.rlim_cur = newSize;
            setrlimit(RLIMIT_CORE, &limit);
        }
        break;
    case 'd':
        if (getcwd(path, PATH_MAX) == NULL)
            fprintf(stderr, "Could not determine the current directory");
        else
            printf("Current directory is %s.\n", path);
        break;
    case 'v':
        printf("Environment variables:\n");
        for (char** p = environ; *p; p++)
            printf ("%s\n", *p);
        break;
    case 'V':
        if (putenv(option.argument) != 0)
            fprintf(stderr, "Failed to change the value of the environment variable");
        break;
        
    } 
}

int main(int argc, char** argv){
    int character;
    char* listOfOptions = "ispucdvV:U:C:";
    char* withArguments = "V:U:C:";
    Option newOption;

    initStack(5);
    opterr = 0;

    while  ((character = getopt(argc, argv, listOfOptions)) != EOF)  {
        if (strchr(listOfOptions, character) != NULL) {
            newOption.type = character;
            if (strchr(withArguments, character) != NULL)
                newOption.argument = optarg;
            else 
                newOption.argument = NULL;
            push(newOption);
        } 
        else if (character == '?') {
            if (strchr(withArguments, optopt) != NULL)
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (optopt == ' ')
                fprintf (stderr, "Missing option character");
            else
                fprintf (stderr, "Unknown option -%c.\n", optopt);
        }
    }

    int count = options.count;
    for (int i = 0; i < count; ++i)
        executeOption(pop());
    deleteStack();

    return 0;
}