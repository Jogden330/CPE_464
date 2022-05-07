#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef struct Node{

    int data;
    char handle[100];
    struct Node* prev;
    struct Node* next;


} Node;

typedef struct List{

  uint32_t NumClient;
  struct  Node* first;
  struct  Node* last; 


} List;


List* createList();

void insertList(struct List* list, int data, char* handle);

Node* getClientSocket(struct List* list, int socketNum);

Node* getClientHandle(struct List* list, char* handle);

void removeList(struct List* list, struct Node* client);

void printList(struct List * list);
