#include "lab9.h"
/*For this linked list implementation we will be using a doubly-linked list with a dummy head node*/
typedef struct node{
	void * object;
	struct node * next;
	struct node * prev;
}Node;
struct List{
	Node * head;
	int length;
};

//Helper Functions
Node * createNode(void * object);

/*This function inserts the given object at the head of the List (after the dummy head) in O(1) time.
 * It will return 1 on success and 0 for any failure*/
int insertAtHead(List * list, void * object){
	if(list == NULL){
		return 0;
	}
	else{
		Node * new = createNode(object);

		Node * DummyH = list->head;
		Node * TrueH = list->head->next;

		if(list->length == 0){
			DummyH->next = new;
			new->prev = DummyH;

			list->length += 1;

			return 1;
		}
		else{
			new->next = TrueH;
			new->prev = DummyH;

			TrueH->prev = new;
			DummyH->next = new;

			list->length += 1;

			return 1;
		}
	}
	return 0;
}
/*This function will return the object at the tail of the list in O(n) time. My implementation could
 * accomplish this in O(1) but I will make it to do so in O(n)*/
void * getTail(List * list){
	if(list == NULL || list->head->next == NULL){
		return NULL;
	}
	else{
		Node * current = list->head->next;
		if(list->length == 1){
			return current->object;
		}
		else
		{
			while(current->next != NULL){
				current = current->next;
			}
			return current->object;
		}
	}
	return NULL;
}
/*This function removes and returns the object at the tail of the list in O(n) time.*/
void * removeTail(List * list){
	if(list == NULL || list->head->next == NULL){
		return NULL;
	}
	else{
		Node * current = list->head->next;
		if(list->length == 1){
			list->head->next = NULL;
			list->length -= 1;
			return current;
		}
		else{
			while(current->next != NULL){
				current = current->next;
			}
			Node * newTail = current->prev;
			newTail->next = NULL;
			list->length -= 1;

			return current;
		}
	}
	return NULL;
}
/*This function will initialize a List structure, using a dummy head and tail node, and return
 * a List with initialized members*/
List * initList(){
	List * p = malloc(sizeof(List));
	if(p == NULL){
		printf("ERROR: LMF\n");
		return p;
	}
	else{
		p->head = createNode(0);
		p->length = 0;

		return p;
	}
}
/*This function will return the size of the list stored in the length member*/
int getSize(List * list){
	if(list == NULL){
		printf("ERROR: NO LIST\n");
		return -1;
	}
	else{
		return list->length;
	}
}
void * getAtIndex(List * list, int index){
	if(list == NULL){
		printf("ERROR: NO LIST\n");
		return NULL;
	}
	int Size = getSize(list);
	if(index < Size){
		Node * current = list->head->next;
		for(int i = 0; i < index; i++){
			current = current->next;
		}
		return current->object;
	}
	else{
		return NULL;
	}
}
/*This function frees all the memory associated with the linked list*/
void freeList(List * list){
	if(list == NULL){
		return;
	}
	else{
		Node * temp = list->head;
		while(temp){
			list->head = list->head->next;
			free(temp);
			temp = list->head;
		}
		free(list);
		printf("List Freed\n");
	}
}
/*This function will create and initialize a Node with the passed object*/
Node * createNode(void * object){
	Node * new = malloc(sizeof(Node));
	if(new == NULL){
		printf("ERROR: NMF\n");
		return new;
	}
	else{
		new->next = NULL;
		new->prev = NULL;
		new->object = object;

		return new;
	}
}



