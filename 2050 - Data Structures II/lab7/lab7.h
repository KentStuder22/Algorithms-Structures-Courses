#include <stdio.h>
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

List* initList();
int getSize(List *list);
void freeList(List *list);
void* getAtIndex(List *list, int index);

#define SECT_C
int insertAtTail(List *list, void *object);
int numberOf(List *list, void *object);
void* removeBefore(List *list, void *sentinel);
