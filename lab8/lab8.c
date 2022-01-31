#include "lab8.h"

/*This function takes 2 Car structs and compares them based on their color members. 
 * The function will return a negative number if a is "less than" b, a 0 if they 
 * are equal and a positive number if a is greater than b. Im assuming values are calucalted
 * using ASCII addition*/
int compareCars(Car *a, Car *b){
	if(a->color < b->color){
		return -1;
	}
	else if(a->color > b->color){
		return 1;
	}
	else{
		return 0;
	}
}
/*This function takes a list and a pointer to a struct and inserts the struct into the list
 * in sorted descending order using the comparison function. Highest value car at head,
 * lowest value car at tail. Function returns 1 for success, and 0 for failure*/
int insertCarDescending(List *list, Car *car){
	if(list == NULL){
		printf("ERROR: No List\n");
		return 0;
	}
	else{
		Node* new = makeNode((void*)car, NULL);
		Node* current = list->head;
		if(getSize(list) == 0){ //if there is no item in list, we simply just insert at head
			list->head = new;
			++list->size;
			return 1;
		}
		if(getSize(list) == 1){ //if there is just 1 item we compare them
			if(compareCars(car, current->data) > 0){ //if our new item is "larger" we insert at head 
				Node* temp = list->head;
				list->head = new;
				new->next = temp;
			}
			else{
				list->head->next = new;
			}
			++list->size;
			return 1;
		}
		if(compareCars(car, current->data) > 0){ //if our new car is greater than the first one we insert at head
			Node* temp = list->head;
			list->head = new;
			new->next = temp;
			++list->size;
			return 1;
		}
		//If our new car is valued less than the first we need to traverse until it is not
		while(current->next != NULL){
			if(compareCars(car, current->next->data) < 0){
				current = current->next;
			}
			else if(compareCars(car, current->next->data) > 0){
				Node* temp = list->head;
				current->next = new;
				new->next = temp;
				++list->size;
				return 1;
			}
			else{
				return 0;
			}
		}
		//This is the tail case:
		current->next = new;
		++list->size;
		return 1;
	}
	return 0;
}
/*This function takes a list containing car structs and prints out all the structs*/
void printCars(List *list){
	if(list == NULL){
		printf("ERROR: No List\n");
		return;
	}
	else{
		for(int i = 0; i < getSize(list); i++){
			Car* temp = getAtIndex(list, i);
			printf("Car Color: %c\tVIN: %lld\tMileage: %lf\n", temp->color, temp->VIN, temp->mileage);
		}
	}
}
/*This function takes a list containing car structs and returns the number of structs that are deemed
 * "less than" the one passed in the second parameter*/
int countAllLesserCars(List *list, Car *car){
	if(list == NULL){
		printf("ERROR: No List\n");
		return -1;
	}
	else{
		int count = 0;
		Node* current = list->head;
		while(current != NULL){
			if(compareCars(car, current->data) > 0){
				count++;
			}
			current = current->next;
		}
		return count;
	}
}

