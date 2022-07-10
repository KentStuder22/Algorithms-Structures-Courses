#include <stdio.h>
#include <stdlib.h>

#define SECT_C
typedef struct {
	char color; // R, G, B, Y
	long long VIN;
	double mileage;
} Car;

char getColor(Car *car);
void setColor(Car *car, char color);
long long getVIN(Car *car);
void setVIN(Car *car, long long VIN);
double getMiles(Car *car);
void setMiles(Car *car, double miles);
void printCarArray(Car *cars);
Car* loadCarsFromFile(const char *filename);
Car* getCarWithLeastMiles(Car *cars);
Car* getCarByVIN(Car *cars, long long VIN);
char getMostCommonColor(Car *cars);
