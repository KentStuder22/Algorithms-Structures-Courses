//Kent Studer (krspcp - 12544417)
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/sstring.h"

/* This function takes a character array and determines if it is null terminated. If the string
    is null terminated then the function will return true, if not return false */
bool string_valid(const char *str, const size_t length)
{
    if(str == NULL || length == 0) //input validation
    {
        return false;
    }
    else
    {
        if(*(str + (length - 1)) != '\0') //if last char is NOT the null term, return false
        {
            return false;
        }
        return true; //null terminated string, return true
    }
}
/* This function will take a string str, and length of the string, then copy the number of characters
    upto the length parameter into a heap based char array which will be returned to the user. Else NULL */
char *string_duplicate(const char *str, const size_t length)
{
    if(str == NULL || length == 0) //input validation
    {
        return NULL;
    }
    else
    {
        //Phrase "upto" means up to not including the length-th character in the string
        //so we do not have to include an additional char for the null termiantor (0-based)
        char *copy_string = (char*)malloc(length * sizeof(char)); 
        if(copy_string == NULL)
        {
            return NULL;
        }
        else
        {
            char *copy_val = strcpy(copy_string, str); //strcpy return pointer, if null strcpy has failed
            if(copy_val == NULL)
            {
                return NULL;
            }
            return copy_string; //successfull copy
        }
    }
}
/* This function will simply compare str_a with str_b and determine if they are equal in character order and length. 
    Will return true for success and false if the strings are either invalid or are not equal */
bool string_equal(const char *str_a, const char *str_b, const size_t length)
{
    if(str_a == NULL || str_b == NULL || length == 0) //input validation
    {
        return false;
    }
    else
    {
        //to verify that the 2 strings are equal we can just use strcmp
        int is_equal = strcmp(str_a, str_b);
        if(is_equal != 0) //strcmp will only return 0 if the 2 strings are equal
        {
            return false;
        }
        return true;
    }
}
/* This function will calculatre the length of string str, given max length of the string, and return it as an integer.
    If the string is invalid or an error occurs, the function will return -1 */
int string_length(const char *str, const size_t length)
{
    if(str == NULL || length == 0) //input validation
    {
        return -1;
    }
    else
    {
        int count = 0; //variable to count valid chars
        while(*str != '\0')
        {
            count++; //inc count
            str++; //move pointer by 1
        }
        return count;
    }
}
/* This function will tokenize the string str, with delimiter delims, of string with a max length of length. The tokenized strings will be stored in
    the pre-alllocated array tokens, limited by the max_token_length param (including the null term), as well as the requested_tokens which tells how many
    tokens are possible from the given string */
int string_tokenize(const char *str, const char *delims, const size_t str_length, char **tokens, const size_t max_token_length, const size_t requested_tokens)
{
    if(str == NULL || delims == NULL || str_length == 0 || tokens == NULL || max_token_length == 0 || requested_tokens == 0) //input validation
    {
        return 0;
    }
    else
    {
        int i;
        for(i = 0; i < requested_tokens; i++) //check if our tokens array is allocated right
        {
            if(tokens[i] == NULL)
            {
                return -1;
            }
        }

        int token_count = 0;
        char *copy_string = (char*)malloc(str_length * sizeof(char)); //because string is passed as constant we need a copy
        if(copy_string == NULL)
        {
            return -1;
        }
        else    
        {
            copy_string = strcpy(copy_string, str); //copied const str
            if(copy_string == NULL)
            {
                return -1;
            }
            else
            {
                char *token = strtok(copy_string, delims);
                while(token != NULL) //strok will return null once it has no more tokens
                {
                    if(token_count > requested_tokens) //max tokens exceeded
                    {
                        return -1;
                    }
                    if(strlen(token) > max_token_length) //max token length exceeded
                    {
                        return -1;
                    }
                    strcpy(tokens[token_count], token); //copy into array
                    token = strtok(NULL, delims); //take the next token

                    token_count++;
                }
                return token_count;
            }
        }
        return -1;
    }
}
/* This function will take a string str and a pointer to an int, and convert the str to and integer value which will be reflected in 
    converted_value. Will return true for successful conversion or false for error or failed conversion */
bool string_to_int(const char *str, int *converted_value)
{
    if(str == NULL || converted_value == NULL || strlen(str) > 10) //input validation
    {
        return false;
    }
    else
    {
        *converted_value = atoi(str); //convert to integer using atoi
        if(converted_value <= 0) //if conversion failed 
        {
            return false;
        }
        return true; //conversion successful
    }
}
