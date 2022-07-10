#include <stdio.h>
#include <stdlib.h>

#define COMMON_2
typedef struct {
	void **array;
	int size;
	int maxSize;
	int startIndex;
} OffsetList;

OffsetList* initOffsetList(int maxSize, int startIndex);
int getOffsetListSize(OffsetList *list);
void freeOffsetList(OffsetList *list);
void* getOAtIndex(OffsetList *list, int index);

#define SECT_C
int insertAtEnd(OffsetList *list, void *object);
int hasObjects(OffsetList *list);
int numberOf(OffsetList *list, void *object);
void* removeFromStart(OffsetList *list);
