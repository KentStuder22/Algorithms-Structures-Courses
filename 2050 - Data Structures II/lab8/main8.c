#include "lab8.h"
#include "LinkedList.h"

int main(){
	List *list = initList();
	if(list != NULL){
		printf("List INIT Complete\n");
	}

	Car* Car1 = malloc(sizeof(Car));
	Car1->color = 'R';
	Car1->VIN = 1233252;
	Car1->mileage = 25000;

	Car* Car2 = malloc(sizeof(Car));
	Car2->color = 'G';
	Car2->VIN = 12482347;
	Car2->mileage = 100000;

	Car* Car3 = malloc(sizeof(Car));
	Car3->color = 'B';
	Car3->VIN  = 14982472;
	Car3->mileage = 35469;

	Car* Car4 = malloc(sizeof(Car));
	Car4->color = 'Y';
	Car4->VIN = 134894102;
	Car4->mileage = 12500;

	Car* Car5 = malloc(sizeof(Car));
	Car5->color = 'B';
	Car5->VIN = 13474284;
	Car5->mileage = 55650;

	insertCarDescending(list, Car1);
	insertCarDescending(list, Car2);
	insertCarDescending(list, Car3);
	insertCarDescending(list, Car4);
	insertCarDescending(list, Car5);

	printCars(list);

}
	   	
