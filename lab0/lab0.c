#include "lab0.h"

/* REQUIRED CODE GOES HERE */
int countOccurrences(int size, int array[], int query)
{
	int count = 0;
	for(int i = 0; i < size; i++)
	{
		if(array[i] == query)
		{
			count++;
		}
	}
	return count;
}

