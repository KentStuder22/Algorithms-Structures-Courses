#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../include/sys_prog.h"

uint32_t getConverted(uint32_t);

// LOOK INTO OPEN, READ, WRITE, CLOSE, FSTAT/STAT, LSEEK
// GOOGLE FOR ENDIANESS HELP

/* This function will take a string as a file name, a void pointer representing the destination of the file data,
    and offset describing how far into the file I will begin reading the object and the length of the destination pointer memory 
    The function will return true for success and false for error or failure */
bool bulk_read(const char *input_filename, void *dst, const size_t offset, const size_t dst_size)
{
    if(input_filename == NULL || dst == NULL || dst_size == 0 || offset > dst_size) //input validation
    {
        return false;
    }
    else
    {
        int file_open = open(input_filename, O_RDONLY); //returns file descriptor used if successful and -1 if not
        if(file_open == -1)
        {
            return false;
        }
        else
        {
            lseek(file_open, offset, SEEK_SET); //move offset of the file before reading
            read(file_open, dst, dst_size);
            close(file_open);
            if(errno == -1)
            {
                return false;
            }
            return true;
        }
    }
}
/* This function does the reverse of bulk_read in that it will take data (src) and write it to file with filename output_filename starting
    at the given offset and ending when bit length reaches src_size. Returns true for success and false for error or failure */
bool bulk_write(const void *src, const char *output_filename, const size_t offset, const size_t src_size)
{
    if(src == NULL || output_filename == NULL || src_size == 0) //input validation
    {
        return false;
    }
    else
    {
        //everything is similar to bulk_read except we write instead
        int file_open = open(output_filename, O_WRONLY);
        if(file_open == -1)
        {
            return false;
        }
        else
        {
            lseek(file_open, offset, SEEK_SET);
            write(file_open, src, src_size);
            close(file_open);
            if(errno == -1)
            {
                return false;
            }
            return true;
        }
    }
}
/* This function will take a file name and stores the files metadata into the metadata struct */
bool file_stat(const char *query_filename, struct stat *metadata)
{
    if(query_filename == NULL || metadata == NULL) //input validation
    {
        return false;
    }
    else
    {
        int file_open = open(query_filename, O_RDONLY); //open the file
        if(file_open == -1)
        {
            return false;
        }
        else
        {
            int meta_err = fstat(file_open, metadata); //puts the files meta data into the struct
            if(meta_err < 0)
            {
                return false;
            }
            return true;
        }
    }
}
/*Converts all the data in src_data to either big/little endian depending on what it is and return true for success
    or false for error or failure */
bool endianess_converter(uint32_t *src_data, uint32_t *dst_data, const size_t src_count)
{
    if(src_data == NULL || dst_data == NULL || src_count == 0) //input validation
    {
        return false;
    }
    else
    {
        int i;
        for(i = 0; i < src_count; i++) //loop through src array
        {
            uint32_t temp = getConverted(src_data[i]); //for each value convert it & store in dst array
            dst_data[i] = temp;
        }
        return true;
    }
}
//Function to handle all the appropraite endian conversions
uint32_t getConverted(uint32_t data)
{
    int far_left_byte, middle_left_byte, middle_right_byte, far_right_byte;
    int res;

    far_left_byte = (data & 0x000000FF) >> 0;
    far_left_byte <<= 24;

    middle_left_byte = (data & 0x0000FF00) >> 8;
    middle_left_byte <<= 16;

    middle_right_byte = (data & 0x00FF0000) >> 16;
    middle_right_byte <<= 8;

    far_right_byte = (data & 0xFF000000) >> 24;
    far_right_byte <<= 0;

    return (res = far_left_byte | middle_left_byte | middle_right_byte | far_right_byte);
}

