#include <stdio.h>
#include <stdlib.h>

#define SECT_C
double* readDoubleFromFile(FILE *file, int *length);
void freeDoubleArray(double **array);
