#include "lab10.h"
//The queue structure will use dummy nodes at both head and tail

typedef struct nodestruct{
	void * object;
	int priority; //we will use ASCII values of color to determine order
	struct nodestruct * next;
	struct nodestruct * prev;
}Node;
struct Queue{
	Node * head;
	Node * tail;
	int length;
};
Node * createNode(void * object, int priority){
	Node * new = malloc(sizeof(Node));
	if(new == NULL){
		printf("ERR at Null Malloc\n");
		return new;
	}else{
		new->object = object;
		new->priority = priority;
		new->next = NULL;
		new->prev = NULL;

		return new;
	}
}
//This function initializes a Queue structure in O(1) time
Queue * initPQ(){
	Queue * p;
	p = malloc(sizeof(Queue));
	if(p == NULL){
		printf("ERR at Init\n");
		return p;
	}else{
		p->head = createNode(0, 0);
		p->tail = createNode(0, 10000);
		p->length = 0;

		return p;
	}
}
//This function frees the queue in O(N) time
void freePQ(Queue * pq){
	if(pq == NULL){
		printf("ERR No list\n");
		return;
	}else{
Node * temp = pq->head;
		while(temp){
			pq->head = pq->head->next;
			free(temp);
			temp = pq->head;
		}
		free(pq);
	}
}
//This function inserts the Car object in Ascending priority based on its ASCII values of its color
int insertCarAscendingPQ(Queue * pq, Car * car){
	if(pq == NULL || car == NULL){
		printf("ERR at parameters\n");
		return -1;
	}else{
		int priority = car->color;
		Node * new = createNode(car, priority);
		Node * current = pq->head;
		if(pq->length == 0){
			new->next = pq->tail;
			new->prev = pq->head;
			pq->head->next = new;
			pq->tail->prev = new;

			pq->length += 1;

			return 1;
		}else{
			while(current->next != NULL){
				if(current->next->priority <= new->priority){
					current = current->next;
				}
				new->next = current->next;
				new->prev = current;
				current->next->prev = new;
				current->next = new;
				
				pq->length += 1;

				return 1;
			}
		}
		return -1;
	}
}
//This function retrieves the car with the Max priority of the queue in O(1) time
Car * peekMaxCar(Queue * pq){
	if(pq == NULL || pq->length == 0){
		printf("ERR at queue\n");
		return NULL;
	}else{
		Node * highest = pq->tail->prev;
		return highest->object;
	}
}
//This function removes the object with the highest priority in O(1) time
Car * deQueueMaxCar(Queue * pq){
	if(pq == NULL || pq->length == 0){
		printf("ERR at queue\n");
		return NULL;
	}else{
		Node * Remove = pq->tail->prev;
		Node * Connect = pq->tail->prev->prev;

		Connect->next = pq->tail;
		pq->tail->prev = Connect;

		return Remove->object;
	}
}

	
