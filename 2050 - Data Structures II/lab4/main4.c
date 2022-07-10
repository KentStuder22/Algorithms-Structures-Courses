#include "lab4.h"

int main() {
	int size = 15;
	float* array = createArray(size, sizeof(float), sizeof(long long));
	for(int i = 0; i < size; i++){
		array[i] = i + 20;
	}
	printf("Array:\n");
	for(int j = 0; j < size; j++){
		printf("Array[%d] = %.2f\n", j, array[j]);
	}
	array[5] = 13; //creating a new minimum value for testing in middle of array
	printf("Array[5] = %.2f\n", array[5]);
	findArrayMin(array);
	long long min = getArrayMin(array);
	printf("Array[14] = %.2f\n", array[14]); //checking to see that we did not write over last element
	printf("The minimum after changing element 5 is %lld\n", min);
	freeArray(array);

}

