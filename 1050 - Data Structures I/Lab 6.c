//Kent Studer, CS 1050 Section E, Lab 6, 23 March 2020//

#include <stdio.h>

void GetInput( char *string, int *outputIntArray, int *size );
void PrintOutput( char *string, int *outputIntArray );

int main( void )
{
    //Declaring necessary variables//
    int maxlength = 100;
    char string[ maxlength ];
    int size = 100;
    int array[ size ];
    int *sizePtr = &size;

    //Welcome Message//
    printf("******************\n");
    printf("*Welcome to Lab 6*\n");
    printf("******************\n");
    printf("\n");

    //Getting user input//
    GetInput( string, array, sizePtr );

    printf("\n");

    //PrintOutput//
    PrintOutput( string, array );

}

void GetInput( char *string, int *outputIntArray, int *size )
{
    int i = 0;
    int sentinel = -1;

    printf("Please enter a string: ");
    scanf("%s", string);
    
    printf("\n");

    printf("Please enter positive integers, and -1 to end\n");
    
    while( *outputIntArray != sentinel && outputIntArray < size )
    {
        printf("\tElement #%d: ", i);
        outputIntArray++;
        scanf("%d", outputIntArray);
        i++;
    }
}

void PrintOutput( char *string, int *outputIntArray )
{
    int i;

    printf("The string you entered:\n");
    printf("%s\n", string);
    
    printf("\n");

    printf("Array elements you entered:\n");
    for( i = 0; outputIntArray[ i ] != -1; i++ )
    {
        printf("\tArray[%d] = %d\n", i, outputIntArray[ i + 1 ]);
    }

}
