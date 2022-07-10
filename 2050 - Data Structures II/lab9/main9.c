#include "lab9.h"

int main(){
	List * list = initList();
	if(list != NULL){
		printf("List init Successful\n");
	}
	for(int i = 0; i < 15; i++){
		float * obj = malloc(sizeof(float*));
		float val = (float)i*2.345;
		*obj = val;
		int error = insertAtHead((void*)obj, list);
		if(error == 1){
			printf("Head insert complete\n");
		}
	}
}
