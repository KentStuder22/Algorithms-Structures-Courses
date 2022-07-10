#include "lab2.h"

/*This function will read a file of double numbers into a malloced array using the given pointer to a file
 * The first number in the file will indicate the size of the array.
 * Input:
 * 	file: FILE pointer that will be used for reading
 * 	length: a int pointer that we will update with the size of the array
 * Output:
 * 	A populated double array with the given values read from the file */
double* readDoubleFromFile(FILE *file, int* length)
{
	int size;
	double* arr;

	/*scanning the first number into our size variable*/
	fscanf(file, "%d\n", &size);

	/*Allocating memory based on said size*/
	arr = malloc(size*sizeof(double));
	if(arr == NULL)
	{
		printf("ERROR: Malloc Failed\n");
		return NULL;
	}
	else //if malloc was successful
	{
		/*Looping through array and scanning each value into it*/
		for(int i = 0; i < size; i++)
		{
			fscanf(file, "%lf\n", &(arr[i]));
		}
	}
	*length = size; //Setting our pointer to our size for calling use
	return arr;
}

/*This function will take a double pointer and frees the memory that it points to. It will also set the pointer
 * to NULL to avoid any further referencing from the calling of the freed array
 * Input:
 * 	array: pointer to an array of double (**)
 * Output:
 * 	Array is freed, and the variable array in the calling program is now set to NULL*/
void freeDoubleArray(double** array)
{
	if(array == NULL) //error checking to make sure we get a valid array passed
	{
		return;
	}
	else
	{
		free(*array); //dereferencing the double pointer here so we can appropriately free the array
		*array = NULL; //allows us to set the calling variable to null
	}
}
