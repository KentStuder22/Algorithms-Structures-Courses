#include "lab5.h"
/*Helper Functions*/
int getSize(void* Array);

/*These are the getter and setter functions for each member of the struct. The functions will take a pointer
 * to a struct and either set the given memeber value or return said value*/
void setColor(Car* car, char color){
	car->color = color;
}
void setVIN(Car* car, long long VIN){
	car->VIN = VIN;
}
void setMiles(Car* car, double miles){
	car->mileage = miles;
}
char getColor(Car* car){
	return car->color;
}
long long getVIN(Car* car){
	return car->VIN;
}
double getMiles(Car* car){
	return car->mileage;
}
/*This function takes an array of Car structures and prints out each structure on a newline,
 * struct memebers are labeled and separated by commas*/
void printCarArray(Car* cars){
	printf("Cars:\n");
	for(int i = 0; i < getSize(cars); i++){
		printf("Color: %c, VIN: %lld, Miles: %lf\n", cars[i].color, cars[i].VIN, cars[i].mileage);
	}
}
/*This function will read Car structs in from a file, and store then in a malloced array. The memory allocated
 * will include an integer spot for the size of the array "hidden" before the actual contents. The first
 * number in the file will be interpreted as the number of car structures within. */
Car* loadCarsFromFile(const char* filename){
	FILE* fp = fopen(filename, "r");
	if(!fp){
		printf("ERROR: Could no open file\n");
		fclose(fp);
		return NULL;
	}
	int size;
	Car* array;

	fscanf(fp, "%d\n", &size);
	printf("Size: %d\n", size);

	array = malloc(size*sizeof(Car) + sizeof(int));
	if(array == NULL){
		printf("ERROR: Malloc Failed\n");
		return NULL;
	}
	*((int*)array) = size;
	array = (Car*)((int*)array + 1);

	for(int i = 0; i < size; i++){
		fscanf(fp, "%c,%lld,%lf\n", &array[i].color, &array[i].VIN, &array[i].mileage);
	}
	fclose(fp);

	return array;
}
/*This function takes an array of Car structs and returns a pointer to the Car with the least amount of miles*/
Car* getCarWithLeastMiles(Car* cars){
	double least = cars[0].mileage;
	for(int i = 1; i < getSize(cars); i++){
		if(cars[i].mileage < least){
			least = cars[i].mileage;
		}
	}
	for(int i = 0; i < getSize(cars); i++){
		if(cars[i].mileage == least){
			return &(cars[i]);
		}
	}
	return NULL;
}
/*This function will take an array of Car structs and return a pointer to the car with the specified VIN number*/
Car* getCarByVIN(Car* cars, long long VIN){
	for(int i = 0; i < getSize(cars); i++){
		if((cars + i)->VIN == VIN){
			return (cars + i);
		}
	}
	printf("No Cars were found with that VIN\n");
	return NULL;
}
/*This function takes an array of Car structs and returns the most common color of all the cars,
 * R, G, B, Y, it will return the Character Z if no clear winner has been found*/
char getMostCommonColor(Car* cars){
	int RCount = 0;
	int GCount = 0;
	int BCount = 0;
	int YCount = 0;
	for(int i = 0; i < getSize(cars); i++){
		if(cars[i].color == 'R'){
			RCount++;
		}
		else if(cars[i].color == 'G'){
			GCount++;
		}
		else if(cars[i].color == 'B'){
			BCount++;
		}
		else if(cars[i].color == 'Y'){
			YCount++;
		}
	}
	if(RCount > GCount && RCount > BCount && RCount > YCount){
		return 'R';
	}
	else if(GCount > RCount && GCount > BCount && GCount > YCount){
		return 'G';
	}
	else if(BCount > RCount && BCount > GCount && BCount > YCount){
		return 'B';
	}
	else if(YCount > RCount && YCount > GCount && YCount > BCount){
		return 'Y';
	}
	return 'Z';
}
/*Helper functions*/
int getSize(void* array){
	return *((int*)array - 1);
}


