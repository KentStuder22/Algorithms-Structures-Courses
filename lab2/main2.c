#include "lab2.h"

void printDoubleArray(double *array, int size) {
	printf("[ ");
	for(int i = 0; i < size; ++i) {
		printf("%lf%s", array[i], ((i + 1 == size) ? "" : ", "));
	}
	printf(" ]\n");
}

int main() {
	FILE* ptr = fopen("floats.txt", "r");
	if(!ptr)
	{
		printf("The file was not successfully opened\n");
		return -1;
	}
	int length;
	double* array = readDoubleFromFile(ptr, &length);

	printf("The length is now %d\n", length);

	printDoubleArray(array, length);

	if(array != NULL)
	{
		printf("Array is still populated\n");
	}

	freeDoubleArray(&array);

	if(array != NULL)
	{
		printf("NULL set not successful\n");
	}
	else
	{
		printf("Null set successful\n");
	}

}
