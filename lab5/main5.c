#include "lab5.h"
void freeArray(void* array);
int main(void){
	const char* filename = "cars.txt";
	Car* Array = loadCarsFromFile(filename);
	if(Array == NULL){
		printf("ERROR: Array not created\n");
		return -1;
	}
	printCarArray(Array);
	
	Car* Least = getCarWithLeastMiles(Array);
	printf("Least: Color = %c, VIN = %lld, Mileage = %lf\n", Least->color, Least->VIN, Least->mileage);

	Car* VIN = getCarByVIN(Array, 482302849);
	printf("Searching for VIN: 482302849\n");
	printf("VIN: Color = %c, VIN = %lld, Mileage = %lf\n", VIN->color, VIN->VIN, VIN->mileage);

	printf("Most Common Color: %c\n", getMostCommonColor(Array));

	setColor(&(Array[0]), 'B');
	setColor(&(Array[1]), 'B');
	printf("Changing 2 Red Cars to Blue\n");

	printf("Most Common Color: %c\n", getMostCommonColor(Array));

	freeArray(Array);
}
void freeArray(void* array){
	array = (void*)((int*)array - 1);
	free(array);
}

