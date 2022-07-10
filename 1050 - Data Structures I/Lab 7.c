//Kent Studer, CS 1050 Section E, Lab 7, April 7, 2020//
#include <stdio.h>
//Prototpying functions//
void GetString( char *string );
void StringSegment( char *string, int segment );
void ReverseString( char *string );

int main( void )
{
    int maxlength = 255;
    char string[ maxlength ];
    int segment = 0;

    //Welcome message//
    printf("*** Welcome to Lab 7 ***\n");

    //Function to get string and print it//
    GetString( string );

    //Function to determine how many segments (of 3)//
    StringSegment( string, segment );

    //Function to reverse the segments//
    ReverseString( string );

    //Thank you message//
    printf("*** Thank you for doing Lab 7 ***\n");
}

void GetString( char *string )
{
    printf("Please enter a string: ");
    scanf("%s", string);
    printf("You entered: %s\n", string);
}

void StringSegment( char *string, int segment )
{
    int i;

    for( i = 0; string[ i ] != '\0'; i++ )
    {
        if( i % 4 == 0 )
        {
            segment++;
        }
    }
    printf("There are %d segments in the string\n", segment);
}

void ReverseString( char *string )
{
    char Reverse[ 255 ];
    int end = 0;
    int begin = 0;
    int count = 0;
    int segment = 0;
    //int *beginPtr = &begin;
    //int *endPtr = &end;

    while( string[ count ] != '\0' )
    {
        count++;

        if( count % 4 == 0 )
        {
            segment++;
            end = segment;

            for( begin = 0; begin < count; begin++ )
            {      
                Reverse[ begin ] = string[ end ];
                end--;
            }
        }
    }

    Reverse[ begin ] = '\0';
    printf("The modified string is: %s\n", Reverse);
}


