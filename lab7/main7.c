#include "lab7.h"
int main(){
	List* list = initList();
	if(list != NULL){
		printf("List Init Complete\n");
	}

	for(int i = 0; i < 15; i++){
		float* obj = malloc(sizeof(float*));
		float val = (float)i*2.345;
		*obj = val;
		int error = insertAtTail((void*)obj, list);
		if(error == 1){
			printf("Object Inserted at Tail\n");
		}
	}
	printf("List has %d objects\n", getSize(list));

	for(int i = 0; i < getSize(list); i++){
		float* temp = (float*)getAtIndex(list, i);
		printf("List #%d: %.2f\n", i, *temp);
	}
	freeList(list);
	printf("List Freed\n");
}
