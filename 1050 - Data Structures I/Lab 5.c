// Lab 5, Kent Studer, Section E, 3 March 2020 //

#include <stdio.h>

int InitializeArray( int array[], int begin, int end, int increment);
int AddArray( int array[], int size, int array2[], int size2,int array3[]);
void PrintArray(int array[], int size, int array2[], int size2);
void PrintAddArray(int array[], int size);

int main()
{
    int FirstArray[256];
    int SecondArray[256];
    int ThirdArray[256];

    int FourthArray[256];
    int FifthArray[256];
    int SixthArray[256];

    int FirstCount = InitializeArray( FirstArray, 3, 30, 3);
    int SecondCount = InitializeArray( SecondArray, 5, 50, 5);
    int ThirdCount = AddArray( FirstArray, FirstCount, SecondArray, SecondCount, ThirdArray);

    int FourthCount = InitializeArray( FourthArray, 40, 0, 4);
    int FifthCount = InitializeArray( FifthArray, 80, 0, 8);
    int SixthCount = AddArray( FourthArray, FourthCount, FifthArray, FifthCount, SixthArray);

    printf("First Input Arrays:\n");
    PrintArray( FirstArray, FirstCount, SecondArray, SecondCount );
    
    printf("First Ouput Array:\n");
    PrintAddArray( ThirdArray, ThirdCount ); 

    printf("Second Input Arrays:\n");
    PrintArray( FourthArray, FourthCount, FifthArray, FifthCount );

    printf("Second Output Array:\n");
    PrintAddArray( SixthArray, SixthCount );


    return 0;
}

int InitializeArray( int array[], int begin, int end, int increment)
{
    int InitializeCount = 0;

    if( begin < end )
    {
        for( int v = begin; v <= end; v += increment)
        {
            array[InitializeCount] = v;
            InitializeCount++;
        }
    }
    else
    {
        for( int v = begin; v >= end; v -= increment)
        {
            array[InitializeCount] = v;
            InitializeCount++;
        }
    }
    return InitializeCount;
}

int AddArray( int array[],int size,  int array2[], int size2, int array3[] )
{
    int i;
    for( i = 0; i < size && i < size2; i++)
    {
        array3[i] = array[i] + array2[i];
    } 
    return i;
}

void PrintArray( int array[], int size, int array2[], int size2 )
{
    for( int i = 0; i < size && i < size2; i++ )
    {
        printf("Array 1 Element %d = %d\t", i, array[i]);
        printf("Array 2 Element %d = %d\n", i, array2[i]);
    }
}

void PrintAddArray( int array[], int size )
{
    for( int i = 0; i < size; i++)
    {
        printf("Array 1 Element %d = %d\n", i, array[i]);
    }
}


