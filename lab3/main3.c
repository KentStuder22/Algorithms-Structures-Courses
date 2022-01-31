#include "lab3.h"

int main() {
	FILE *fp = fopen("meteorShowers.txt", "r");
	if(!fp)
	{
		printf("ERROR: File not found\n");
		return -1;
	}

	int* array;
	int startIn;
	int size;

	array = loadMeteorShowers(fp, &startIn, &size);
	printf("Array:\n");
	for(int i = startIn; i < size+startIn; i++)
	{
		printf("Array[%d] = %d\n", i, array[i]);
	}
	printf("These values should be garbage:\n");
	printf("%d\t%d\t%d\t%d\t%d\n", array[0], array[1], array[2], array[3], array[4]);
	printf("the fifth index is the true start of the scanned in array: %d\n", array[5]);
	freeIntArray(&array, startIn);
	if(array == NULL)
	{
		printf("Array NULL set successful\n");
	}
	else
	{
		printf("Array NULL set unsuccessful\n");
	}
}

