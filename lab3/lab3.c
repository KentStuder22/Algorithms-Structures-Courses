#include "lab3.h"

/*This function will read integers from a file into a malloc array. The first number in the file
 * is the size of the file, and the second number will be the starting index of the array
 * the pointer of the array will then point to the number indicated in the second line of the text
 * Inputs:
 * 	file: FILE pointer "meteors.txt"
 * 	startIndex: we will store the value of the starting index in this int pointer
 * 	length: we will store the value of the length in this int pointer
 * Outputs:
 * 	An array that correctly points to the indicated starting index and updated startIndex
 * 	and length values for the calling*/
int* loadMeteorShowers(FILE *file, int* startIndex, int* length)
{
	int* arr;
	int size; //temp variables for the calling
	int index;

	fscanf(file, "%d\n%d\n", &size, &index); //first we scanf the first 2 values into respective temps
	
	arr = malloc(size*sizeof(int)); 
	if(arr == NULL) //error check
	{
		printf("ERROR: Malloc Failed\n");
		return NULL;
	}

	arr = arr - index; //we move the pointer back to ensure we scan in correctly with our starting index

	for(int i = index; i < size+index; i++) //scanning values into array current arr points to -5 spots before 
						//correct memory
	{
		fscanf(file, "%d\n", &(arr[i]));
	}

	printf("The size is %d\n", size);
	printf("The starting index is %d\n", index);

	*startIndex = index; //storing values back into calling pointers
	*length = size;

	return arr;
}
/*This function will take a double pointer (**) of our array and our starting index
 * To correctly free this we will use pointer arithmetic to add the index back so we 
 * correctly free the right memory location. It will then set the array variable to NULL
 * to prevent further access*/
void freeIntArray(int** array, int startIndex)
{
	if(array == NULL)
	{
		return;
	}
	else
	{
		free(*array + startIndex);
		*array = NULL;
	}
}
