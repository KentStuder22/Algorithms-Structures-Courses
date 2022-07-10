#include "lab7.h"
//Helper Function(s)
Node* createNode(void* object){
	Node* new = malloc(sizeof(Node));
	if(new == NULL){
		printf("ERROR: Node Malloc Failed\n");
		return new;
	}
	else{
		new->next = NULL;
		new->data = object;
		return new;
	}
}

/*This function initializes and returns a linked list*/
List* initList(){
	List* list = malloc(sizeof(List));
	if(list == NULL){ //error check
		printf("ERROR: LMF\n");
		return list;
	}
	else{ //set head node to NULL and initialize size to 0
		list->head = NULL;
		list->size = 0;
		return list;
	}
}
/*This function takes a list pointer and returns the number of elements in the list
 * stored in the "size" member, or -1 for an error*/
int getSize(List* list){
	if(list == NULL){ //error check
		printf("ERROR: No List\n");
		return -1;
	}
	else{ //returning the value stored in the size memeber
		return (list->size);
	}
}
/*This function takes a linked list and frees all memory for the list*/
void freeList(List* list){
	if(list == NULL){ //error check
		printf("ERROR: No List\n");
		return;
	}
	else{
		Node* temp = list->head; //temp node for freeing
		while(temp){ //while temp is not NULL
			list->head = list->head->next; //move head to the next node
			free(temp); //free where head used to be
			temp = list->head; //set temp to the next node in the list
		}
		free(list); //free the list object
		printf("List Free Successful\n");
	}
}
/*This function takes a linked list and returns the object at the given index, or NULL
 * if there was an error*/
void* getAtIndex(List* list, int index){
	if(list == NULL){ //error check
		printf("ERROR: No List\n");
		return NULL;
	}
	int Size = getSize(list); 
	if(index < Size){ //if index is valid
		Node* current = list->head; //node for moving through list
		for(int i = 0; i < index; i++){
			current = current->next;
		}
		return current->data; //return the object at the given index (0 based)
	}
	else{
		return NULL;
	}
	return NULL;
}
/*This function takes a linked list and inserts the given object at the end of the list
 * return 1 for success or 0 for failure*/
int insertAtTail(List* list, void* object){
	if(list == NULL){ //error check
		printf("ERROR: No List\n");
		return 0;
	}
	else{
		Node* new = createNode(object); //create new node with passed object
		if(new != NULL){ //if new node is valid
			Node* current = list->head; //temp to hold head address
			if(getSize(list) == 0){ //if 0 objects we insert at head
				list->head = new;
				list->size += 1;
				return 1;
			}
			while(current->next != NULL){ //moving to the last node in said list
				current = current->next;
			}
			current->next = new; //the last nodes next pointer now points to the new node, effective tail insert
			list->size += 1;
			return 1;
		}
		else{
			return 0;
		}
	}
	return 0;
}
/*This function takes a linked list and returns the number of times the object occurs in the linked list*/
int numberOf(List* list, void* object){
	if(list == NULL){ //error check
		printf("ERROR: No List\n");
		return 0;
	}
	else{
		int count = 0; //count for holding how many occurences
		if(getSize(list) == 0){
			printf("No Items in list\n");
			return 0;
		}
		else{
			Node* current = list->head; //node to traverse list
			while(current != NULL){
				if(current->data == object){ //if our current nodes data is the same as our object, we add to count
					count++;
				}
				current = current->next; //traverse list
			}
			return count;
		}
		return 0;
	}
}
/*This function takes a linked list and removes the object that appears before the specified 
 * "sentinel" object in the list. If the sentinel does not exist, nothing is removed*/
void* removeBefore(List* list, void* sentinel){
	if(list == NULL){ //error check
		printf("ERROR: No List\n");
		return NULL;
	}
	else{
		if(getSize(list) == 0){ //if no items we return NULL
			printf("No items in list\n");
			return NULL;
		}
		Node* current = list->head; //traveling node
		for(int i = 0; i < getSize(list); i++){
			if(current->next->next->data == sentinel){ //because we are removing the node PRIOR to a sentinel we have to check 2 nodes ahead for proper removal
				Node* remove = current->next;
				current->next = current->next->next;
				list->size--;
				return remove;
			}
		}
		return NULL;
	}
}
				

