//Kent Studer, CS 1050 Section E, 28 April 2020, Lab 10 Command Line and Structures//

#include <stdio.h>
#include <string.h>

struct movie
{
    char title[30];
    double gross;
    int year;
};

void printSorted( struct movie list[] );
void bsort( struct movie list[], char *argument[] );
void Alphasort( struct movie list[] );
void bsortFB( struct movie list[], char *argv[], char *argv2[] );

void Alphasort( struct movie list[] )
{
    int i;
    int j;
    struct movie temp;

    for( i = 0; i < 10 - 1; i++ )
    {
        for( j = 0; j < (10 - 1 - i); j++ )
        {
            if( list[j].title[0] > list[j + 1].title[0])
            {
                temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }
}

/*Simple print function for structure*/
void printSorted( struct movie list[] )
{
    int i;
    printf("Sorted Movies:\n");
    printf("%35s%20s%15s\n", "Title", "Gross", "Year");
    for( i = 0; i < 10; i++ )
    {
        printf("%35s%20.0lf%15d\n", list[i].title, list[i].gross, list[i].year);
    }
}

/*Sorting based on only category*/
void bsort( struct movie list[], char *argument[] )
{
    int i;
    int j;
    struct movie temp;

    for( i = 0; i < (10 - 1); i++ )
    {
        for( j = 0; j < (10 - 1 - i); j++ )
        {
            if( strcmp( *argument, "TITLE") == 0 )
            {
                Alphasort( list );
            }
            else if( strcmp( *argument, "GROSS") == 0 )
            {
                if( list[j].gross > list[j + 1].gross )
                {
                    temp = list[j];
                    list[j] = list[j + 1];
                    list[j + 1] = temp;
                }
            }
            else if( strcmp( *argument, "YEAR") == 0 )
            {
                if( list[j].year > list[j + 1].year )
                {
                    temp = list[j];
                    list[j] = list[j + 1];
                    list[j + 1] = temp;
                }
            }
        }
    }
}

void bsortFB( struct movie list[], char *argument[], char *argument2[] )
{
    int i;
    int j;
    struct movie temp;

    for( i = 0; i < (10 - 1); i++ )
    {
        for( j = 0; j < (10 - 1 - j); j++ )
        {
            if( strcmp( *argument2, "Forward") == 0 )
            {
                bsort( list, argument );
            }
            else if( strcmp( *argument2, "Backward") == 0 )
            {
                if( strcmp( *argument, "TITLE") == 0 )
                {
                    Alphasort( list );
                }
                else if( strcmp( *argument, "GROSS") == 0 )
                {
                    if( list[j].gross < list[j + 1].gross )
                    {
                        temp = list[j];
                        list[j] = list[j + 1];
                        list[j + 1] = temp;
                    }
                }
                else if( strcmp( *argument, "YEAR") == 0 )
                {
                    if( list[j].year < list[j + 1].year )
                    {
                        temp = list[j];
                        list[j] = list[j + 1];
                        list[j + 1] = temp;
                    }
                }
            }
        }
    }
}


/*Main Function*/
int main( int argc, char *argv[] )
{
    struct movie array[10];
    FILE *mPtr;

    if(( mPtr = fopen("prelab10.dat", "r")) == NULL)
    {
        printf("File could not be opened\n");
    }
    else
    {
        /*Looping through each line*/
        for( int i = 0; i < 10; i++ )
        {
            fscanf( mPtr, "%s%lf%d", array[i].title, &array[i].gross, &array[i].year );
        }
    }

    if( strcmp( argv[1], "prelab10.dat") != 0 )
    {
        printf("***Error: Cannot open file %s", argv[1]);
        printf("\n");
    }
    if( argc == 2 )
    {
        Alphasort( array );
        printSorted( array );
    }
    if( argc == 3 )
    {
        bsort( array, argv + 2 );
        printSorted( array );
    }
    if( argc == 4 )
    {
        bsortFB( array, argv + 2, argv + 3 );
        printSorted( array );
    }
    fclose( mPtr );
}




