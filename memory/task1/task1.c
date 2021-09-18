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
    long newUlimit;
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
    

    } 
}

int main(int argc, char** argv){
    int character;
    char* listOfOptions = "ispuV:U:";
    char* withArguments = "V:U:";
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
            if (strchr(withArguments, optopt) != NULL) {
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            }
            else
                fprintf (stderr, "Unknown option -%c.\n", optopt);
        }
    }

    int size = options.count;
    for (int i = 0; i < size; ++i)
        executeOption(pop());
    
    deleteStack();
    return 0;
}