#include "LinkedList.h"

int isValidIndex(List *list, int index) {
	return index >= 0 && index < list->size;
}

// Allocates space for a Node struct, and returns a pointer to the allocated memory
Node* makeNode(void *data, Node *next) {
	Node *node = malloc(sizeof(Node));
	if(node) {
		*node = (Node){ next, data };
	}
	return node;
}

// Frees the given node struct pointer, and returns the data contained within
void* freeNode(Node *node) {
	void *temp = node->data;
	free(node);
	return temp;
}

// returns the node at the requested index, or NULL on failure
void* nodeAtIndex(List *list, int index) {
	if(!isValidIndex(list, index)) {
		return NULL;
	}
	Node *cursor = list->head;

	/* We don't need to check that cursor->next is !NULL, because
	 * we already know that the index is valid for the length of this list
	 */
	while(index--) {
		cursor = cursor->next;
	}

	return cursor;
}

/*	clearList
 * 		Frees all of the allocated memory for the nodes in the
 * 		given list. The list is still usable after the return of
 * 		this function, and the list size will be set to 0.
 * 	Parameters:
 * 		list: The list to cleared
 */
void clearList(List *list) {
	Node *cursor = list->head;
	while(cursor) {
		Node *temp = cursor;
		cursor = cursor->next;
		free(temp);
	}
	list->head = NULL;
	list->size = 0;
}

// Sets the head of the list to the given node, and returns the old head
Node* setHead(List *list, Node *newHead) {
	Node *oldHead = list->head;
	list->head = newHead;
	return oldHead;
}

// Returns the index of the given object on the list, or -1
int indexOf(List *list, void *object) {
	Node *cursor = list->head;
	int count = 0;
	while(cursor) {
		if(cursor->data == object) {
			return count;
		}
		++count;
		cursor = cursor->next;
	}
	return -1;
}

/* inserts the given object on the list at the given index. Returns
 * 1 on success and 0 on failure. Note that list->size is a valid
 * index, as it represents the end of the list.
 */
int insertAtIndex(List *list, void *object, int index) {
	if(!isValidIndex(list, index) && (index != list->size)) {
		return 0;
	} else if(!index) {
		Node *new = makeNode(object, NULL);
		if(!new)
			return 0;
		new->next = setHead(list, new);
	} else {
		Node *previous = nodeAtIndex(list, index - 1);
		Node *new = makeNode(object, previous->next);
		if(!new)
			return 0;
		previous->next = new;
	}
	++list->size;
	return 1;
}

// Removes and returns the object at the given index, returns NULL on failure
void* removeAtIndex(List *list, int index) {
	if(!isValidIndex(list, index)) {
		return NULL;
	}
	void *object;
	if(!index) {
		object = freeNode(setHead(list, list->head->next));
	} else {
		Node *previous = nodeAtIndex(list, index - 1);
		Node *temp = previous->next;
		previous->next = temp->next;
		object = freeNode(temp);
	}
	--list->size;
	return object;
}

// Initializes a Linked List
// returns a pointer to the list on success, or NULL on failure
List* initList() {
	List *list = malloc(sizeof(List));
	if(list) {
		*list = (List){ NULL, 0 };
	}
	return list;
}

// returns the size of the given list
int getSize(List *list) {
	return list->size;
}

// returns the object at the requested index, or NULL on failure
void* getAtIndex(List *list, int index) {
	Node *node = nodeAtIndex(list, index);
	if(node) {
		return node->data;
	}
	return NULL;
}

// frees all memory allocated to the list
void freeList(List *list) {
	clearList(list);
	free(list);
}
