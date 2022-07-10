// Kent Studer, 3/10/2020, Lab 5.1, Computer Science 1050 section E //
#include <stdio.h>
#define COLS 3
#define ROW 9
#define SecondCol 4

void Print2DArray(int array[][ COLS ], int rows);
void PrintGradeCut(int array[][ SecondCol ], int rows);
char CalcLetterGrade(int array[][ COLS ], int rows, char array3[][ COLS ]);
void PrintFinalGrade(char array[][ COLS ] , int rows);

int main( void )
{
    int array1 [ ROW ][ COLS ] = {{72,68,62},{95,88,95},{93,97,86},{98,77,98},{99,92,90},{47,32,27},{97,99,85},{85,95,91},{76,82,98}};
    int array2 [ COLS ][ SecondCol ] = {{90,80,70,60},{85,75,65,55},{90,70,60,40}};
    char array3 [ ROW ][ COLS ] = {{'C','C','C'},{'A','A','A'},{'A','A','B'},{'A','B','A'},{'A','A','A'},{'F','F','F'},{'A','A','B'},{'B','A','A'},{'C','B','A'}}; 

    printf("Here are the scores:\n");
    Print2DArray( array1, ROW );
    
    printf("Here are the grade cutoffs:\n");
    PrintGradeCut( array2, COLS );

    //CalcLetterGrade( array1, ROW, array3 );

    printf("Here are the grades:\n");
    PrintFinalGrade( array3, ROW );
    
}

void Print2DArray(int array[][ COLS ], int rows)
{
    int i;
    int j;
    for( i = 0; i < ROW; i++ )
    {
        printf("Row %d: ", i);
        for( j = 0; j < COLS; j++ )
        {
            printf("%d   ", array[ i ][ j ]);
        }
        printf("\n");
    }
}
void PrintGradeCut(int array[][ SecondCol ], int rows)
{
    int i;
    int j;
    for( i = 0; i < COLS; i++ )
    {
        printf("Row %d: ", i);
        for( j = 0; j < SecondCol; j++ )
        {
            printf("%d   ", array[ i ][ j ]);
        }
        printf("\n");
    }
}
char CalcLetterGrade(int array[][ COLS ], int rows, char array3[][ COLS ])
{
    char grade = 'F';
    for( int i = 0; i < ROW; i++)
    {
        if( array[ i ][ 0 ] >= 90 )
        {
            grade = 'A';
        }
        else if( array[ i ][ 1 ] >= 80 && array[ i ][ 1 ] <= 89 )
        {
            grade = 'B';
        }
        else if( array[ i ][ 2 ] >= 70 && array[ i ][ 2 ] <= 79 )
        {
            grade = 'C';
        }
        else if( array[ i ][ 3 ] >= 60 && array[ i ][ 3 ] <= 69 )
        {
            grade = 'D';
        }
        else
        {
            grade = 'F';
        }
    }

    for( int j = 0; j < ROW; j++ )
    {
        if( array[ j ][ 0 ] >= 85 )
        {   
            grade = 'A';
        }
        else if( array[ j ][ 1 ] >= 75 && array[ j ][ 1 ] <= 84 )
        {
            grade = 'B';
        }
        else if( array[ j ][ 2 ] >= 65 && array[ j ][ 2 ] <= 74 )
        {
            grade = 'C';
        }
        else if( array[ j ][ 3 ] >= 55 && array[ j ][ 3 ] <= 64 )
        {
            grade = 'D';
        }
        else
        {
            grade = 'F';
        }
    }
    for( int k = 0; k < ROW; k++ )
    {
        if( array[ k ][ 0 ] >= 90 )
        {
            grade = 'A';
        }
        else if( array[ k ][ 1 ] >= 70 && array[ k ][ 1 ] <= 89 )
        {
            grade = 'B';
        }
        else if( array[ k ][ 2 ] >= 60 && array[ k ][ 2 ] <= 69 )
        {
            grade = 'C';
        }
        else if( array[ k ][ 3 ] >= 40 && array[ k ][ 3 ] <= 59 )
        {
            grade = 'D';
        }
        else
        {
            grade = 'F';
        
        }
    }

    return grade;
}

void PrintFinalGrade(char array[][ COLS ], int rows)
{
    int i;
    int j;
    for( i = 0; i < ROW; i++)
    {
        printf("Learner %d: ", i);
        for( j = 0; j < COLS; j++)
        {
            printf("%c   ", array[ i ][ j ]);
        }
        printf("\n");
    }
}



