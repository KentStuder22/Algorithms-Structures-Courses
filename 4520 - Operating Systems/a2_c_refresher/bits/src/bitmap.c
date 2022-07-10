//Kent Studer (krspcp - 12544417)
#include "../include/bitmap.h"
#include <math.h>

// data is an array of uint8_t and needs to be allocated in bitmap_create
//      and used in the remaining bitmap functions. You will use data for any bit operations and bit logic
// bit_count the number of requested bits, set in bitmap_create from n_bits
// byte_count the total number of bytes the data contains, set in bitmap_create

/* This function simply creates a bipmap of n_bits size all initalizaed to 0. Will return a pointer
    to the bitmap if successful or NULL on failure */
bitmap_t *bitmap_create(size_t n_bits)
{
    if(n_bits == 0) //input validation
    {
        return NULL;
    }
    else
    {
        //because bits are 1/32 of an int, we need to convert them down for our malloc
        bitmap_t *new_map = malloc(sizeof(bitmap_t));
        if(new_map == NULL)
        {
            return NULL;
        }
        else
        {
            new_map->bit_count = n_bits; //setting fields of the struct 
            double bytes = n_bits / 8.0;
            new_map->byte_count = ceil(bytes); //1 byte = 8 bits so convert and use ceiling division

            new_map->data = (uint8_t*)malloc((n_bits / 8) * sizeof(uint8_t));
            if(new_map->data == NULL)
            {
                return NULL;
            }
            else
            {
                int i;
                for(i = 0; i < (n_bits / 8); i++) //loop through the indexes setting the full int to 0
                {
                    new_map->data[i] = 0; //init all the "int" to 0 which sets all the bits to 0 respectively
                }
            }
            return new_map;
        }
    }
}
/* This function takes a bitmap and the desired bit to set and will return true for success and false for failure */
bool bitmap_set(bitmap_t *const bitmap, const size_t bit)
{
	if(bitmap == NULL || bit > bitmap->bit_count || bit < 0) //input validation
    {
        return false;
    }
    else
    {
        //to do this we need to find what index in the array the bit lies
        int index = bit / 8;

        //the bit position in the index is going to be bit % 32
        int bit_position = bit % 8;

        //we use a flag to set the individual bit, then just shift it the value of the bit position
        unsigned int flag = 1;
        flag = flag << bit_position;

        bitmap->data[index] = bitmap->data[index] | flag;

        return true;
    }
}
/* This function will reset (or set to 0) the bit parameter specified in the bitmap array. Will return true for success
    or false for failure */
bool bitmap_reset(bitmap_t *const bitmap, const size_t bit)
{
    if(bitmap == NULL || bit > bitmap->bit_count || bit < 0)
    {
        return false;
    }
    else
    {
        //find index of array
        int index = bit / 8;

        //find position in the index
        int bit_position = bit % 8;

        unsigned int flag = 1;
        flag = flag << bit_position;
        flag = ~flag;

        bitmap->data[index] = bitmap->data[index] & flag;

        return true;
    }
}
/* This function will essentially take a bitmap and a bit, and find the current state of that bit,
    returning it as either true or false */
bool bitmap_test(const bitmap_t *const bitmap, const size_t bit)
{
    if(bitmap == NULL || bitmap->data == NULL || bit > bitmap->bit_count || bit < 0) //input validation
    {
        return false;
    }
    else
    {
        //index of array
        int index = bit / 8;

        //position within the index
        int bit_posistion = bit % 8;

        unsigned int flag = 1;

        flag = flag << bit_posistion;

        //if the value at the index and the flag are equal we have a bit set to 1, else 0
        if(bitmap->data[index] & flag)
        {
            return true;
        }
        else
        {
            return false;
        }
        return false;
    }
}
/* This function takes a bitmap and finds the first instance of a set value (1), it will return the address of the
    set bit if found, if not SIZE_MAX will be returned */
size_t bitmap_ffs(const bitmap_t *const bitmap)
{
    if(bitmap == NULL || bitmap->data == NULL)
    {
        return SIZE_MAX;
    }
    else
    {
        int i;
        for(i = 0; i < bitmap->bit_count; i++) //loop through all the bits
        {
            unsigned int temp_flag = 1;
            int index = i / 8;
            int bit_position = i % 8;
            temp_flag = temp_flag << bit_position;

            if(bitmap->data[index] & temp_flag)
            {
                return i;
            }
        }
        return SIZE_MAX; //not found, return SIZE_MAX
    }
}
/* This function will effectively do the same as ffs, except instead of searching for the address of the first set bit
    it will look for the address of the first 0 bit, returning SIZE_MAX if error or could not find */
size_t bitmap_ffz(const bitmap_t *const bitmap)
{
    if(bitmap == NULL || bitmap->data == NULL)
    {
        return SIZE_MAX;
    }
    else
    {
        int i;
        for(i = 0; i < bitmap->bit_count; i++) //loop through all the bits
        {
            unsigned int temp_flag = 1; //for this we need to find 0
            int index = i / 8;
            int bit_position = i % 8;
            temp_flag = temp_flag << bit_position;

            if(bitmap->data[index] & temp_flag)
            {
                continue;
            }
            else
            {
                return i;
            }
        }
        return SIZE_MAX; //not found, return SIZE_MAX
    }
}

bool bitmap_destroy(bitmap_t *bitmap)
{
    if(bitmap == NULL || bitmap->data == NULL) //if both dont exist, no need to free, return false
    {
        return false;
    }
    else
    {
            free(bitmap->data); //free the array pointer
            free(bitmap); //free the struct pointer

            return true;
    }
}
