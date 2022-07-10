#include <stdio.h>

int main( void )
{
    int Start;
    int Limit;
    int Increment;
    int Count;
do{
        printf("Enter Start: ");
        scanf("%d", &Start);
}while( Start > 100 || Start < 1 );
    
do{
       printf("Enter Limit: ");
       scanf("%d", &Limit);
}while( Limit > 100 || Limit < 1 );

if( Start > Limit)
{
    do{
        printf("Enter Increment: ");
        scanf("%d", &Increment);
    }while( Increment >= 0 );

    for(Count = Start; Count >= Limit; Count = Count + Increment)
    {
        printf("%d\t", Count);
    }
}
else if( Start < Limit)
{
    do{
        printf("Enter Increment: ");
        scanf("%d", &Increment);
    }while( Increment <= 0 );
   
   for(Count = Start; Count <= Limit; Count = Count + Increment)
   {
       printf("%d\t", Count);
   }
}

return 0;


}



