//Kent Studer (krspcp - 12544417)
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "../include/error_handling.h"
/* Adding input validation as well as allocation verification checks */
int create_blank_records(Record_t **records, const size_t num_records)
{
	int uns_nrecords = (int)num_records;
	if(num_records == 0  || *records != NULL)
	{
		return -1;
	}
	else
	{
		//passing a negative size_t will result in a memory chunk allocation that is far to 
		//large, we need to cast it to an unsigned prior to malloc and check
		*records = (Record_t*) malloc(sizeof(Record_t) * num_records);
		if(*records == NULL || uns_nrecords < 0)
		{
			return -2;
		}
		memset(*records,0,sizeof(Record_t) * num_records);
		return 0;
	}	
}
/* Adding input validation that would result in -1 returned, file errors = -2, and any other 
	errors including the reading of the data = -3 */
int read_records(const char *input_filename, Record_t *records, const size_t num_records) 
{
	if(input_filename == NULL || records == NULL || num_records == 0) //input validation
	{
		return -1;
	}
	else
	{
		int fd = open(input_filename, O_RDONLY);
		if(fd == -1) //if open fails it returns -1
		{
			return -2;
		}
		ssize_t data_read = 0;
		for (size_t i = 0; i < num_records; ++i) 
		{
			data_read = read(fd,&records[i], sizeof(Record_t));	
		}
		//to check whether the right amount of data is read we can divide the return from read by 
		//the size of record_t to get a validation amount
		if((((size_t)data_read)/sizeof(Record_t)) > num_records || errno == -1)
		{
			return -3;
		}
		return 0;
	}
}
/* will create a new record, returning -1 for any bad parameter passing, and -2 for any failed
	allocation. 0 will be returned if all is validated and successful */
int create_record(Record_t **new_record, const char* name, int age)
{
	if(*new_record != NULL || name == NULL || strlen(name) > 50 || age < 1 || age > 200
		|| strcmp(name, "\n") == 0) //input validation
	{
		return -1;
	}
	else
	{
		*new_record = (Record_t*) malloc(sizeof(Record_t));
		if(*new_record == NULL) //malloc validation
		{
			return -2;
		}
		memcpy((*new_record)->name,name,sizeof(char) * strlen(name));
		(*new_record)->name[MAX_NAME_LEN - 1] = 0;	
		(*new_record)->age = age;
		return 0;
	}
}
