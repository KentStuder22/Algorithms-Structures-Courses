// lab 4, Comp Sci 1050, Spring 2020, Kent Studer, 25 Feb 2020 //
#include<stdio.h>
#include<math.h>

// function to calculate log of any base b of any number x //
double logAny( double x, double b)
{
  return log10(x)/log10(b);
}

// main function //
int main( void )
{
   double x;
   // counter for x from 1 to 4096 by multiplier 4 //
    for(x = 1; x <= 4096; x *= 4)
    {
      //display x //
      printf("Number %lf:\n", x);

       double b;
        // counter for b from 2 to 128 by multiplier 4 //
        for(b = 2; b <= 128; b *= 4)
        {  
            //display the results of the log base b of x calculation // 
            printf("\t log base %lf = %lf\n", b, logAny(x,b));
        }
    }
        
    return 0;
}
