//Kent Studer (krspcp - 12544417)
#include "../include/allocation.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* This function will allocate an array of nmembers that are each member_size long. If the 
    clear flag is set to 1, it will clear the memory using calloc, else it will use malloc.
    will return a void pointer to the array */
void* allocate_array(size_t member_size, size_t nmember,bool clear)
{
    if(member_size == 0 || nmember == 0) //input validation
    {
        return NULL;
    }
    else
    {
        void *array;
        if(clear == 1) //flag set, use calloc
        {
            array = (void*)calloc(nmember, member_size);
            if(array == NULL) //error check for correct allocation
            {
                return NULL;
            }
            return array;
        }
        else //flag is false or 0, use malloc
        {
            array = (void*)malloc(nmember * member_size);
            if(array == NULL)
            {
                return NULL;
            }
            return array;
        }
        return NULL;
    }
}
/* This function will reallocate a chunk of memory pointer to by ptr, to the size of the size parameter
    and will return the pointer to the original chunk, which could be exactly the same if error occurs */
void* reallocate_array(void* ptr, size_t size)
{
    if(size <= 0) //basically means to free the memory
    {
        free(ptr); //freeing frees the memory but the pointer will still be not-NULL
        ptr = NULL;
        return ptr;
    }
    else //we are reallocating some positive integer
    {
        ptr = realloc(ptr, size);
        return ptr;
    }
}
/* This function will essentially serve as a "wrapper" for free but will also set the received pointer
    to NULL as well */
void deallocate_array(void** ptr)
{
    if(*ptr == NULL) //input validation
    {
        return;
    }
    else //if pointer is valid free the memory then set it to null
    {        
        free(*ptr);
        *ptr = NULL;
        return;
    }
}
/* This function takes a string to represent the filename and reads a single line into a buffer which
    has been allocated on the heap */
char* read_line_to_buffer(char* filename)
{
    if(filename == NULL) //input validation
    {
        return NULL;
    }
    else
    {
        const char* copy = (const char*)filename;
        FILE *fp;
        fp = fopen(copy, "r");
        char *buffer = (char*)malloc(256 * sizeof(char)); //buffer of standard array size

        if(fp == NULL) //file could not open
        {
            free(buffer);
            return NULL;
        }
        else if(buffer == NULL) //malloc failed
        {
            fclose(fp);
            return NULL;
        }
        else
        {
            char *getLine = fgets(buffer, 256, fp);
            fclose(fp);
            if(getLine == NULL) //read failed
            {
                free(buffer);
                return NULL;
            }
            return buffer;
        }  
    }
}
