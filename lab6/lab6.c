#include "lab6.h"
/*This function will initialize and return a pointer to an OffsetList structure
 * and will set the passed parameters to the correct memebers in our structure*/
OffsetList* initOffsetList(int maxSize, int startIndex){
	OffsetList* list = malloc(sizeof(OffsetList)); //give the pointer enough space for a list object
	if(list == NULL){ //error check
		printf("ERROR: List Malloc Failed\n");
		return list;
	}
	else{ //initializers
		list->array = calloc(maxSize* sizeof(void*), sizeof(void*)); //giving array memory space
		list->size = 0;
		list->maxSize = maxSize;
		list->startIndex = startIndex;
		list->array = list->array - startIndex; //set our array pointer back for starting index

		return list;
	}
}
/*This function takes a pointer to an offset list structure and returns the value stored
 * in the size (number of elements) member, returning -1 for failure*/
int getOffsetListSize(OffsetList* list){
	if(list == NULL || list->array == NULL){
		printf("ERROR: No list\n");
		return -1;
	}
	else{
		return list->size;
	}
}
/*This function takes an OffsetList pointer and will free every pointer in the array within
 * the structure, and then free the pointer to the list itself*/
void freeOffsetList(OffsetList* list){
	if(list == NULL || list->array == NULL){
		printf("ERROR: No list\n");
		return;
	}
	else{
		list->array = list->array + list->startIndex; //add back offset for correct free
		for(int i = 0; i < list->maxSize; i++){
			free(list->array[i]); //every element in array is a pointer that needs freeing
		}
		free(list); //gotta free the space given to list object as well
	}
}
/*This function takes an OffsetList and returns the object at that given index, or NULL upon
 * and error*/
void* getOAtIndex(OffsetList* list, int index){
	if(list == NULL || list->array == NULL){
		printf("ERROR: No list\n");
		return NULL;
	}
	if(index < 0 || index >= list->size){
		printf("ERROR: Index out of Bounds\n");
		return NULL;
	}
	void* temp = list->array[index];
	return temp;
}
/*This function takes an pointer to an OffsetList structure and attempts to insert the given object
 * at the tail of the list, returning 1 for success or 0 for failure*/
int insertAtEnd(OffsetList* list, void* object){
	if(list == NULL){
		printf("ERROR: No list\n");
		return 0;
	}
	else{
		int size = getOffsetListSize(list);
		list->array = list->array + list->startIndex; //offset pointer again so we correct insert in memory
		if(size != list->maxSize){
			list->array[size] = object; 
			list->size++;
			list->array = list->array - list->startIndex;
			return 1;
		}
		else{
			list->array = list->array - list->startIndex;
			return 0;
		}
	}
	list->array = list->array - list->startIndex; //setting our pointer back to the offset
	return 0;
}
/*This function takes a pointer to an offsetList structure and returs 1 if the list contains objects
 * or 0 if the list is empty, or -1 if an error has occurred*/
int hasObjects(OffsetList* list){
	if(list == NULL || list->array == NULL){
		printf("ERROR: No list\n");
		return -1;
	}
	if(list->size == 0){
		return 0;
	}
	else{
		return 1;
	}
}
/*This function takes a pointer to an OffsetList structure and returns the number of times an object
 * occurrs within that list*/
int numberOf(OffsetList* list, void* object){
	if(list == NULL || list->array == NULL){
		printf("ERROR: No list\n");
		return -1;
	}
	else{
		int Count = 0;
		list->array = list->array + list->startIndex;//array points to true 0
		for(int i = 0; i < getOffsetListSize(list); i++){
			if(list->array[i] == object){
				Count++;
			}
		}
		list->array = list->array - list->startIndex;//setting pointer back
		return Count;
	}
	return 0;
}
/*This function takes a pointer to an OffsetList structure and removes the object at index 0 from the array*/
void* removeFromStart(OffsetList* list){
	if(list == NULL || list->array == NULL){
		printf("ERROR: No list\n");
		return NULL;
	}
	else{
		list->array = list->array + list->startIndex; //array points to true 0
		void* temp = list->array[0];
		for(int i = 0; i < getOffsetListSize(list); i++){
			list->array[i] = list->array[i+1]; //set every element back one
		}
		list->size = list->size - 1;
		return temp;
	}
}


