// CS 3050 - Programming Assignment 1
// Micro-Facebook
// Creator: Kent Studer (krspcp - 12544417)
// To implement this we will create an array of People structs, each index will correlate to a hash value
// that we will derive using our Person's name. Each Perosn will be given their own unique index on the hash table
// The Person node will then have next & prev pointers which will allow for the addition and deletion of friends

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// we are going to use FNV-1a for our hashing funtion
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

typedef struct pers_node{
    char first_name[15];
    struct pers_node * next;
    struct pers_node * prev;
}Person;

typedef struct list_struct{
    Person * root;
    int friends;
}List;

typedef struct table_struct{
    List * personArray;
    int numPersons;
    int length;
}HashTable;

//initializing a hash table with empty List structures
HashTable *initHashTable(int length){
    HashTable *new = malloc(sizeof(HashTable));
    if(new == NULL){
        return NULL;
    }
    new->numPersons = 0;
    new->length = length;

    new->personArray = malloc(new->length * sizeof(List));
    if(new->personArray == NULL){
        printf("ERROR: Hash Table not allocated\n");
        return NULL;
    }else{
        for(int i = 0; i < new->length; i++){ //initializing each Person list to NULL and friend count to 0
            new->personArray[i].root = NULL;
            new->personArray[i].friends = 0;
        }
        return new;
    }
}

// This function takes a char array (firstName) & initializes a Person struct 
// with that name, setting the next and prev pointers to NULL as well
Person *createPerson(char firstName[]){
    Person *new = malloc(sizeof(Person));
    if(new == NULL){
        printf("ERROR at Person Creation\n");
        return NULL;
    }else{
        new->first_name = firstName;
        new->next = NULL;
        new->prev = NULL;
        return new;
    }
}

// This function will derive a hash value for the passed firstName
// prior to adding to table we will check to determine if the table is > 75% full, if so we increase the table size
// if not we check for collision and add either directly or via linear probing
void addToHash(HashTable *table, char *firstName){
    if(table == NULL || firstName == NULL){
        printf("Null argument passed\n");
        return;
    }else{
        
        Person *new = createPerson(firstName);
        uint64_t hashVal = hashFunction(firstName);
        if(table->numPersons > (.75 * table->length)){ 
            makeBigger(table);
        }
        int key = hashVal % (table->length);

        if(table->personArray[key].root != NULL)){ // if we have a collision we linearly probe until an empty slot is found
        int i;
            for(i = key+1; i < table->length; i++){
                if(table->personArray[i].root == NULL){ // we have found an empty slot
                    table->personArray[i].root = new;
                    break;
                }
                if(i == table->length){ // if we have reached the end of the table we set i to the beginning of the array so we can try and find an empty slot
                    i = 0;
                    continue;
                }
            }
        }
        else{ // the hash value of the table is empty and the Person can be directly added without collision
            hashTable[hashVal].root = new;
            return;
        }
    }
}

// This function will be our primary hashing function which will return a value and modulo that value by the length
// by doing this we ensure that we are only producing hash values within a reasonable range (i.e., the length of the table)
// function is the FNV-1a string hashing function
uint64_t hashFunction(char firstName[], ){
    uint64_t hash = FNV_OFFSET;

    for(int i = 0; i < strlen(firstName); i++){
        hash ^= (uint64_t)(unsigned char)firstName[i];
        hash *= FNV_PRIME;
    }
    return hash;
}

// Supporting functions
// this function will scan through the hash table and count the number of entries are present 
// if the index.root != NULL
int getNumberOfPerson(HashTable * table){
    if(table == NULL){
        printf("ERROR: No table made yet");
        return -1;
    }else{
        return table->numPersons;
    }
}
// This function will take the hasht table and a pointer to the length variable, and make it larger by 2x
void makeBigger(HashTable * table){
    if(hashTable == NULL){
        printf("ERROR: No list made\n");
        return NULL;
    }else{
        int newSize = table->length * 2;
        for(int i = 0; i < )
    }
}
    
