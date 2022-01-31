#include <stdio.h>
#include <stdlib.h>

#define SECT_C
int* loadMeteorShowers(FILE *file, int *startIndex, int *length);
void freeIntArray(int **array, int startIndex);
