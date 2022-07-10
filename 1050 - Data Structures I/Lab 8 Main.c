//Main file for Lab 8, Kent Studer, 14 April 2020
//Prototypes
#include <stdio.h>
#include <ctype.h>

void ShowWords( char *string );

int main( void )
{
    char testString[] = 
    "Once there was a way to get back homeword.\n\
    Once there was a way to get back home.  \n\
    Sleep pretty darling. \n\
    Do not cry, and I will sing this lullabye.";

    ShowWords(testString);
}

