#include "lab1.h"

// Required code goes here

//This function will take a double array and the size of said array and will search for the average of the data 
//that is between the lower and upper bounds provided. 
//Inputs:
//	double* array: array of double values
//	int size: size of the array
//	double* result: pointer to a double that will be changed to the average
//	long floor: lower limit for calculating averages
//	long ceiling: upper limit for calculating averages
//Outputs:
//	1 for success, and 0 if the array does not contain a number within the range. The result pointer
//	will also be changed to reflect the avg, if found, for the calling program to use
int avgBetween(double* array, int size, double* result, long floor, long ceiling)
{
	int count = 0;
	double total = 0;
	
	//This will check whether valid bounds are passed
	if(ceiling <= floor)
	{
		printf("The ceiling has to be greater than the floor\n");
		return 0;
	}

	//This loop will scan through the array. If it is within the bounds (not including the floor and ceiling
	//, we add the value of the array elements to our total and increment a counter for later division
	for(int i = 0; i < size; i++)
	{
		if(array[i] > floor && array[i] < ceiling)
		{
			total = total + array[i];
			count++;
		}
	}
	if(count == 0) //if there are not elements within the given range we will return 0
	{
		printf("There are no values within the bounds given\n");
		return 0;
	}
	else //if there are we calculate the average and return 1
	{
		*result = total/count;
		return 1;
	}
}
