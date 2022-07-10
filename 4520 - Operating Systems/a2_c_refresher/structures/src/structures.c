//Kent Studer (krspcp - 1254417)
#include "../include/structures.h"
#include <stdio.h>

/* This function will compare structure a to b to determine if they are equal
	will return 1 for a match and 0 not */
int compare_structs(sample_t* a, sample_t* b)
{
	if(a == NULL || b == NULL)
	{
		return 0;
	}
	else
	{
		//we dont use memcmp in the case of padding be different between the 2 structs
		if((a->a == b->a) && (a->b = b->b) && (a->c == b->c))
		{
			return 1;
		}
		return 0;
	}
}

void print_alignments()
{
	printf("Alignment of int is %zu bytes\n",__alignof__(int));
	printf("Alignment of double is %zu bytes\n",__alignof__(double));
	printf("Alignment of float is %zu bytes\n",__alignof__(float));
	printf("Alignment of char is %zu bytes\n",__alignof__(char));
	printf("Alignment of long long is %zu bytes\n",__alignof__(long long));
	printf("Alignment of short is %zu bytes\n",__alignof__(short));
	printf("Alignment of structs are %zu bytes\n",__alignof__(fruit_t));
}
/* This function sorts the array of fruits into either apples or oranges.
	will return the size of the array for success or -1 for error */
int sort_fruit(const fruit_t* a,int* apples,int* oranges, const size_t size)
{
	if(a == NULL || apples == NULL || oranges == NULL || size == 0)
	{
		return -1;
	}
	else
	{
		size_t i;
		for(i = 0; i < size; i++)
		{
			if(a[i].type == 1)
			{
				*apples += 1;
			}
			else
			{
				*oranges += 1;
			}
		}
		return size;
	}
	return -1;
}
/* sets the array data types to the correct 1 or 0 depending on how many apples
	or oranges there are */
int initialize_array(fruit_t* a, int apples, int oranges)
{
	if(a == NULL) //input validation
	{
		return -1;
	}
	else
	{
		int total_fruits = apples + oranges;
		int i;
		for(i = 0; i < total_fruits; i++)
		{
			if(i < apples)
			{
				a[i].type = 1;
			}
		}
		return 0;
	}
}
/* give orange an arbitrary type value of 0 */
int initialize_orange(orange_t* a)
{
	if(a == NULL)
	{
		return -1;
	}
	else
	{
		a->type = 0;
		a->weight = 0;
		a->peeled = 0;
		return 0;
	}
}
/* give apple an arbitrary type value of 1 */
int initialize_apple(apple_t* a)
{
	if(a == NULL)
	{
		return -1;
	}
	else
	{
		a->type = 1;
		a->weight = 0;
		a->worms = 0;
		return 0;
	}
}
