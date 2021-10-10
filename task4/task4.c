#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1000

typedef struct Node {
    char* string;
    struct Node* next;
} Node; 

Node* sentinel;
Node* last;

void addNode(char* string, int length) {
    Node* node = malloc(sizeof(Node));
    node->string = malloc(length);
    strcpy(node->string, string);
    node->string[length - 1] = '\0';
    last->next = node;
    last = node;
}

void printNodes() {
    for (Node* node = sentinel->next; node != NULL; node = node->next)
        printf("%s\n", node->string);
}

void freeNodes() {
    Node* temp;
    for (Node* node = sentinel; node != NULL; node = temp) {
        temp = node->next;
        free(node->string);
        free(node);
    }
}

int main() {
    sentinel = malloc(sizeof(Node));
    last = sentinel;
    
    char buffer[BUFFER_SIZE];
    fgets(buffer, BUFFER_SIZE, stdin);
    while(buffer[0] != '.') {
        int length = strlen(buffer);
        addNode(buffer, length);
        fgets(buffer, BUFFER_SIZE, stdin);
    }

    printf("\n");
    printNodes();

    freeNodes();
    return 0;
}