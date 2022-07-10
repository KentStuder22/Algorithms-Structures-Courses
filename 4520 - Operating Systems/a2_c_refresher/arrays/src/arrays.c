//Kent Studer (krspcp - 12544417)
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/arrays.h"

// LOOK INTO MEMCPY, MEMCMP, FREAD, and FWRITE

/* This function will utilize MEMCPY to copy the src array into the dst array returning true if the operation
    was successful and false if failed */
bool array_copy(const void *src, void *dst, const size_t elem_size, const size_t elem_count)
{
    if(src == NULL || dst == NULL || elem_size == 0 || elem_count == 0) //no src or dst array given, throw failure and return
    {
        return false;
    }
    else
    {
        for(int i = 0; i < elem_count; i++)
        {
            dst = memcpy(dst, src, elem_size); //memcpy returns the dst pointer with the src memory copied
            dst = dst + elem_size; //increment both pointers by the size of the element
            src = src + elem_size;
        }
        return true;
    }
}
/* This function will take 2 arrays, data_one & data_two, and compare them. If the 2 arrays contain the same elements,
    the function returns true for success and false for failure */
bool array_is_equal(const void *data_one, void *data_two, const size_t elem_size, const size_t elem_count)
{
    if(data_one == NULL || data_two == NULL || elem_size == 0 || elem_count == 0) //if either of the two arrays are null, we will return failure
    {
        return false;
    }
    else
    {
        for(int i = 0; i < elem_count; i++) 
        {
            if(memcmp(data_one, data_two, elem_size) != 0) //if the current array pointers are not equal, we will return failure
            {
                return false;
            }
            data_one = data_one + elem_size; //current pointers were equal so we increment both and check the next element
            data_two = data_two + elem_size;
        }
        return true; //elements are equal if the for loop completes, return success
    }
}
/* This function will attempt to locate the target element in the data array. If the element is located, the index of 
    said element will be returned, else if the target was not located, the function will return -1 for failure */
ssize_t array_locate(const void *data, const void *target, const size_t elem_size, const size_t elem_count)
{
    if(data == NULL || target == NULL || elem_size == 0 || elem_count == 0) //if our array OR target are null values we return failure
    {
        return -1;
    }
    else
    {
        int index = 0;
        for(int i = 0; i < elem_count; i++)
        {
            if(memcmp(data, target, elem_size) == 0) //if the current array element is the same as the target element, function returns the current index
            {
                return index;
            }
            //Not the same element, increment index and move pointer
            index++; 
            data = data + elem_size;
        }
        return -1; //for loop never returned the index so the target element was not found, return failure
    }
}
/* This function will attempt to write the array, src_data, into a binary file, dst_file. If the writing is successful the function
    will return true, or false for a failed writing */
bool array_serialize(const void *src_data, const char *dst_file, const size_t elem_size, const size_t elem_count)
{
    if(src_data == NULL || dst_file == NULL || elem_size == 0 || elem_count == 0) //if no filename given or size/count are 0 return failure
    {
        return false;
    }
    else
    {
        //Error test to catch whether the filename contains these 3 escape characters
        //can be expanded upon to contain all C escape characters
        char *newline_char = strstr(dst_file, "\n");
        char *tab_char = strstr(dst_file, "\t");
        char *escape_char = strstr(dst_file, "\e");
        if(newline_char != NULL || tab_char != NULL || escape_char != NULL)
        {
            return false;
        }

        FILE *fp = fopen(dst_file, "wb"); //create file pointer with given string
        if(fp == NULL)
        {
            return false;
        }
        else //if file pointer opened successfully, write array to file, close, then return success
        {
            size_t write_check = fwrite(src_data, elem_size, elem_count, fp);
            fclose(fp);
            if(write_check != elem_count) //validating write count
            {
                return false;
            }

            return true;
        }
        return false;
    }
}
/* This function will attempt to read an array from a binary file into the dst_data array. If the reading is successful, the function
    will return true, or false for a failed reading */
bool array_deserialize(const char *src_file, void *dst_data, const size_t elem_size, const size_t elem_count)
{
    if(src_file == NULL || elem_size == 0 || elem_count == 0) 
    {
        return false;
    }
    else
    {
        char *newline_char = strstr(src_file, "\n");
        char *tab_char = strstr(src_file, "\t");
        char *escape_char = strstr(src_file, "\e");
        if(newline_char != NULL || tab_char != NULL || escape_char != NULL)
        {
            return false;
        }

        if(dst_data == NULL) //allocate if the dst array is passed as NULL
        {
            dst_data = malloc(elem_size * elem_count);
            if(dst_data == NULL) //if malloc fails, we return failure
            {
                return false;
            }
        }
        else //all input error checks have been satisfied
        {
            FILE *fp = fopen(src_file, "rb");
            if(fp == NULL) //file open failure, return failure
            {
                return false;
            }
            else
            {
                size_t read_check = fread(dst_data, elem_size, elem_count, fp); //read data from file to array
                fclose(fp);

                if(read_check != elem_count) //validate correct number of elements are read and transferred
                {
                    return false;
                }

                return true; //successful read
            }
        }
        return false;
    }
}

