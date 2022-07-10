#include <stdio.h>

int main( void )

{
    int i;
//using the for loop and i-- to count backwards//
    for(i = 40; i >= 10; i--)
    {
        if(i % 4 == 0 && i % 5 == 0) //this needs to be first our else the program wont get to this point//
        {
            printf( "%d - divisible by 4 and 5\n", i);
        }
       else  if(i % 5 == 0)
        {
            printf( "%d - divisible by 5\n", i);
        }
       else  if(i % 4 == 0)
        {
            printf( "%d - divisible by 4\n", i);
        }
        else //needed to print the numbers not divisible by either of them//
        {
            printf("%d\n", i);
        }
    }

}

