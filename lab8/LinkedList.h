#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>

// partial typedef, so the Node type can contain itself
typedef struct Node Node;

struct Node {
	Node *next;
	void *data;
};

typedef struct {
	Node *head;
	int size;
} List;

int isValidIndex(List *list, int index);
Node* makeNode(void *data, Node *next);
void* freeNode(Node *node);
void* nodeAtIndex(List *list, int index);
void clearList(List *list);
Node* setHead(List *list, Node *newHead);
int indexOf(List *list, void *object);
int insertAtIndex(List *list, void *object, int index);
void* removeAtIndex(List *list, int index);
List* initList();
int getSize(List *list);
void* getAtIndex(List *list, int index);
void freeList(List *list);

#endif
