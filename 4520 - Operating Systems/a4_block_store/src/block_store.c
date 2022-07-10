#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "bitmap.h"
#include "block_store.h"
// include more if you need

// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

/* Need to defined the structure here since it is only declared in the .h file */
typedef struct block_store {
    bitmap_t *bitmap;
    char *data[BLOCK_STORE_NUM_BLOCKS][BLOCK_SIZE_BYTES];
} block_store_t;

/* This function will create a new block store pointer */
block_store_t *block_store_create()
{
    block_store_t *new_store = (block_store_t*)malloc(sizeof(block_store_t));
    if(new_store == NULL)
    {
        return NULL;
    }
    //create the bitmap and copy that memory into the 127th index of our block_store data array
    new_store->bitmap = bitmap_create(BLOCK_STORE_NUM_BLOCKS);
    bitmap_set(new_store->bitmap, BITMAP_START_BLOCK);
    bitmap_set(new_store->bitmap, BITMAP_START_BLOCK + 1);
    memcpy(new_store->data[127], new_store->bitmap, BLOCK_STORE_NUM_BLOCKS);

    return new_store;
}
/* This function will destory the bitmap member of the struct and free the pointer after */
void block_store_destroy(block_store_t *const bs)
{
    if(bs == NULL || bs->bitmap == NULL) //input validation
    {
        return;
    }
    bitmap_destroy(bs->bitmap);
    free(bs);
    return;
}
/* This function will find the first free block, mark it as being in use, and return the 
    blocks ID */
size_t block_store_allocate(block_store_t *const bs)
{
    if(bs == NULL || bs->bitmap == NULL) //input validation
    {
        return SIZE_MAX;
    }
    //find first zero in bitmap if its greater than our blocks or equal to size_max we have error
    size_t first = bitmap_ffz(bs->bitmap);
    if(first == BLOCK_STORE_NUM_BLOCKS || first == SIZE_MAX)
    {
        return SIZE_MAX;
    }
    else
    {
        bitmap_set(bs->bitmap, first);
        return first;
    }
}
/* This function will attempt to allocate the requested block_id, if successful it will return true
    else it will return false */
bool block_store_request(block_store_t *const bs, const size_t block_id)
{
    if(bs == NULL || bs->bitmap == NULL || block_id > BLOCK_STORE_NUM_BLOCKS) //input validation
    {
        return false;
    }
    
    size_t test = bitmap_test(bs->bitmap, block_id);
    if(test == 1) //if the bit has already been set we return false
    {
        return false;
    }
    //bit is not set, set it and check to see if it was successful
    bitmap_set(bs->bitmap, block_id);
    test = bitmap_test(bs->bitmap, block_id);
    if(test == 0) //failure in set
    {
        return false;
    }
    else //successful request
    {
        return true;
    }
}
/* This function will attempt to reset the block with the ID block_id */
void block_store_release(block_store_t *const bs, const size_t block_id)
{
    if(bs == NULL || bs->bitmap == NULL) //input validation
    {
        return;
    }
    bitmap_reset(bs->bitmap, block_id);
}
/* This function will count the number of blocks in use */
size_t block_store_get_used_blocks(const block_store_t *const bs)
{
    if(bs == NULL || bs->bitmap ==NULL)
    {
        return SIZE_MAX;
    }
    else
    {
        size_t set_blocks = bitmap_total_set(bs->bitmap);
        return set_blocks;
    }
}
/* This function will count the number of blocks NOT in use */
size_t block_store_get_free_blocks(const block_store_t *const bs)
{
    if(bs == NULL || bs->bitmap == NULL)
    {
        return SIZE_MAX;
    }
    else
    {
        //first find total number of used and subtract that from total number of blocks
        size_t set_blocks = bitmap_total_set(bs->bitmap);
        size_t not_set = BLOCK_STORE_NUM_BLOCKS - set_blocks;

        return not_set;
    }
}
/* This function will return the total number of blocks used in use or not in use */
size_t block_store_get_total_blocks()
{
    return BLOCK_STORE_NUM_BLOCKS;
}
/* This function will read data from a given block in the block_store and write it to a buffer */
size_t block_store_read(const block_store_t *const bs, const size_t block_id, void *buffer)
{
    if(bs == NULL || bs->bitmap == NULL || bs->data == NULL || block_id > BLOCK_STORE_NUM_BLOCKS || buffer == NULL)
    {
        return 0;
    }
    else
    {
        //copy the data contents into buffer and return length
        memcpy(buffer, bs->data[block_id], BLOCK_STORE_NUM_BLOCKS);
        return BLOCK_STORE_NUM_BLOCKS / 16;
    }
}
/* This function will write data from the specified buffer into the specified block */
size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
    if(bs == NULL || bs->bitmap == NULL || bs->data == NULL || block_id > BLOCK_STORE_NUM_BLOCKS || buffer == NULL)
    {
        return 0;
    }
    else
    {
        //copy the data from buffer into data section of block store
        memcpy(bs->data[block_id], buffer, BLOCK_STORE_NUM_BLOCKS);
        return BLOCK_STORE_NUM_BLOCKS / 16;
    }
}
/* This function will take a file and read all the data from the file into our block_store */
block_store_t *block_store_deserialize(const char *const filename)
{
    if(filename == NULL) //input validation
    {
        return NULL;
    }
    //open file for read only
    int file = open(filename, O_RDONLY);
    if(file < 0)
    {
        return NULL;
    }
    else
    {
        block_store_t *bstore = block_store_create();
        ssize_t read_check = read(file, bstore->data, BLOCK_STORE_NUM_BYTES);
        //we have written data to the block store so flip the bits that we just allocated in it
        bitmap_invert(bstore->bitmap);
        if(read_check != BLOCK_STORE_NUM_BYTES)
        {
            return NULL;
        }

        close(file);
        return bstore;
    }

}
/* This function will take all the data in our block store and write it to a file */
size_t block_store_serialize(const block_store_t *const bs, const char *const filename)
{
    if(bs == NULL || filename == NULL) //input validation
    {
        return 0;
    }
    //tc seems to have removed some permission so we need to delete test.bs everytime
    off_t file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777); 

    //validate file size by seeking to end
    int file_size = lseek(file, 0, SEEK_END);
    if(file_size > (BLOCK_STORE_NUM_BYTES))
    {
        return 0;
    }
    //seek back to beginning
    lseek(file, 0, SEEK_SET);

    //write the data to the file and convert for return
    ssize_t write_check = write(file, bs->data, BLOCK_STORE_NUM_BYTES);
    size_t converted = (size_t)write_check;

    close(file);
    return converted;
}