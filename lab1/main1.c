#include "lab1.h"

void readArray(FILE *file, int size, double *array);
void printArray(double *array, int size);

char *filename = "numbers.txt";

int main() {
    // opening the file
	FILE *file = fopen(filename, "r");
	if(!file) {
		fprintf(stderr, "The file [ %s ] was not successfully opened\n", filename);
		return -1;
	}

    // reading the array size and creating the array
	int size;
	fscanf(file, "%d", &size);
	double array[size];
	readArray(file, size, array);
	printArray(array, size);

    // --- Your function call goes here ---
    	double avg;
	int errorCode = avgBetween(array, size, &avg, 1, 20);
	if(errorCode == 0)
	{
		printf("Error Code: %d\n", errorCode);
	}
	else
	{
		printf("Average: %lf\n", avg);
	}
    
	fclose(file);
}

void readArray(FILE *file, int size, double *array) {
	if(!file) {
		return;
	}
	for(int i = 0; i < size; ++i) {
		fscanf(file, "%lf", (array + i));
	}
}

void printArray(double *array, int size) {
    printf("[ ");
	for(int i = 0; i < size; ++i) {
		printf("%.2lf%s", array[i], ((i + 1 == size) ? "" : ", "));
	}
    puts(" ]");
}
