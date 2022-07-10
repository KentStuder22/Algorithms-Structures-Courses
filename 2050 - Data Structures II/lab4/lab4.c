#include "lab4.h"

/*This function will create an array using dynamic memory allocation. It will take 3 parameters including
 * the size, the size of the elements for the array, and the number of extra bytes specified.
 * Inputs:
 * 	size: integer for number of elements
 * 	elemSize: size_t for the size of the elements
 * 	extraBytes: size_t for the number of extra bytes we are "hidding"
 * Outputs:
 * 	an allocated array with the size hidden before the array and extra bytes stored before */
void* createArray(int size, size_t elemSize, size_t extraBytes){
	int* array = malloc((size*elemSize) + sizeof(int) + (1*extraBytes)); 
	if(array == NULL){
		printf("ERROR: Malloc Failed\n");
		return NULL;
	}
	*(array) = size; //since we are using an int array by default we can do this
	array++; //array now points past the size
	//the extra bytes value is at the end of the array so we dont have to move our pointer anymore

	return (void*)array; 
} 
/*This function takes an array and returns the size of the array that is stored before the true start of the array
 * Input(s):
 * 	array: a void pointer to array that allows for generic array passing
 * Output(s):
 * 	the array size that was hidden prior to the array */
int getArraySize(void* array){
	return *((int*)array - 1);
}
/*This function takes an array and frees the memory associated with the array
 * Input(s):
 * 	array: generic array pointer
 * Output(s):
 * 	the array memory will be freed */
void freeArray(void* array){
	array = (void*)((int*)array - 1);
	free(array);
}
/*This function takes an array of type float, and finds the minimum value within the array, it will then store 
 * the minimum value in the extraBytes that is tagged on at the end of the array
 * Input(s):
 * 	array: an array of floats
 * Output(s):
 * 	the array will have stored the minimum value at the end of the array without writing over the last element 
 * 	or reading past the end of the array */
void findArrayMin(float* array){
	long long min = array[0]; //initializing to the first element
	for(int i = 1; i < getArraySize(array); i++){
		if(array[i] < min){
			min = array[i];
		}
	} //this for loop scans through the array, if the current index value is < the current min, we set mini
	long long* minPtr = (long long*)(array + getArraySize(array));
	*minPtr = min;
}
/*This function will take an array of floats and retrieve the array minimum which is stored in the extra bytes 
 * memory at the END of the array
 * Input(s):
 * 	array: array of floats
 * Ouputs:
 * 	the minimum value stored at the end of the array */
long long getArrayMin(float* array){
	long long *minPtr = (long long*)(array + getArraySize(array));
	return *minPtr;
}

