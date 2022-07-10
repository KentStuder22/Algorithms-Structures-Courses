#include "lab6.h"
int main(){
	int maxSize = 10;
	int offSet = 15;
	OffsetList* list = initOffsetList(maxSize, offSet);
	printf("List init completed\n");

	for(int i = 0; i < maxSize/2; i++){
		double* obj = malloc(sizeof(double*));
		double val = (double)i*2.31;
		*obj = val;
		int error = insertAtEnd((void*)obj, list);
		if(error == 1){
			printf("Object inserted\n");
		}
	}

	for(int i = 0; i < getOffsetListSize(list); i++){
		double* temp = (double*)getOAtIndex(list, i);
		printf("List [%d]: %lf\n",i, *temp);
	}

	printf("The list has %d objects\n", getOffsetListSize(list));
	printf("The lists max size is %d\n", list->maxSize);

	freeOffsetList(list);
}
