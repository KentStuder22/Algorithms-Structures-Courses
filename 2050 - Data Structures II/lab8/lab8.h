#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"

#define SECT_C
typedef struct {
	char color; // R, G, B, Y
	long long VIN;
	double mileage;
} Car;

int compareCars(Car *a, Car *b);
int insertCarDescending(List *list, Car *car);
int countAllLesserCars(List *list, Car *car);
void printCars(List *list);
