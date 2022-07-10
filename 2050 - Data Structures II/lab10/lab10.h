#include <stdio.h>
#include <stdlib.h>

typedef struct Queue Queue;

// Complexity: O(1)
Queue* initPQ();
// Complexity: O(n)
void freePQ(Queue *pq);

/* Priority:
 *	In an ascending priority queue, the "maximum" element is the next to dequeue,
 *	whereas in a descending priority queue, the "minimum" element is next.
 */

 #define SECT_C
 typedef struct {
 	char color; // R, G, B, Y
 	long long VIN;
 	double mileage;
 } Car;

 // Complexity: O(n)
 int insertCarAscendingPQ(Queue *pq, Car *car);
 // Complexity: O(1)
 Car* peekMaxCar(Queue *pq);
 // Complexity: O(1)
 Car* deQueueMaxCar(Queue *pq);
