//Printing all the words in a new line with counter
#include <stdio.h>
#include <ctype.h>

void ShowWords( char *string )
{
    char SplitString[25][25];
    //char *SPtr = string;
    int i;
    int j;
    int charNum = 0;
    int wordNum = 0;


    //Iterating character by character looking for delimiter//
    for( i = 0; string[ i ] != '\0'; i++)
    {
        if( !isspace( string[i] ))
        {
            SplitString[wordNum][charNum] = string[i];
            charNum++;
        }
        else
        {
            SplitString[wordNum][charNum] = '\0';
            wordNum++;
            charNum = 0;
        }
    }
            
    for( i = 0; i < wordNum; i++ )
    {
        printf("Word #%d = %s\n", i, SplitString[i]);
    }

}

