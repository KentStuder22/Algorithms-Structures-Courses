#include <stdio.h>
#include <stdlib.h>

#define SECT_C
void findArrayMin(float *array);
long long getArrayMin(float *array);
void *createArray(int size, size_t elemSize, size_t extraBytes);
int getArraySize(void *array);
void freeArray(void *array);
